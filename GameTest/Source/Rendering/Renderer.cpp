#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer(ECS& ecsReference, MeshLibrary& meshLibraryReference, Camera& cameraReference) : ecs(ecsReference), meshLib(meshLibraryReference), mainCamera(cameraReference)  {}

void Renderer::Init()
{
    mainCamera.position.y = 14.0f;
    SetCameraSpaceToClipSpaceTransform();
    renderQueue.reserve(DEFAULT_BUFFER_SIZE);

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back(&Renderer::ParallelProcessMesh, this, i);
        parallelRenderQueues.emplace_back();
        parallelRenderQueues[i].reserve(DEFAULT_BUFFER_SIZE / threadCount);
    }
}

void Renderer::Render()
{
    ComponentPool<TextureComponent>& textures = ecs.GetTextures();

    SetCameraMatrices();
    worldSpaceToClipSpaceTransform = cameraSpaceToClipSpaceTransform * worldSpaceToCameraSpaceTransform;

    {
        std::lock_guard<std::mutex> frameNumberLock(frameNumberMutex);
        frameNumber++;
    }
    frameNumberCV.notify_one();

    {
        std::unique_lock<std::mutex> numThreadsDoneLock(numThreadsDoneMutex);
        numThreadsDoneCV.wait(numThreadsDoneLock, [this] { return numThreadsDone == threadCount; });
        numThreadsDone = 0;
    }

    for (int i = 0; i < threadCount; i++)
    {
        renderQueue.insert(renderQueue.end(), parallelRenderQueues[i].begin(), parallelRenderQueues[i].end());
    }

    std::sort(renderQueue.begin(), renderQueue.end());
    for (int i = 0; i < renderQueue.size(); i++)
    {
        if (textures.Get(renderQueue[i].entityId).isFilled())
        {
            drawFilledTriangle(renderQueue[i], textures.Get(renderQueue[i].entityId).fill);
        }
        drawTriangle(renderQueue[i], textures.Get(renderQueue[i].entityId).outline);
    }

    renderQueue.clear();
    for (int i = 0; i < threadCount; i++)
    {
        parallelRenderQueues[i].clear();
    }
}

void Renderer::ParallelProcessMesh(int threadId)
{
    ComponentPool<MeshComponent>& meshes = ecs.GetMeshes();
    ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
    long frameNumberIWantToProcess = 1;


    while (true)
    {
        {
            std::unique_lock<std::mutex> frameNumberLock(frameNumberMutex);
            frameNumberCV.wait(frameNumberLock, [this, frameNumberIWantToProcess] { return frameNumber == frameNumberIWantToProcess || frameNumber == SHUTDOWN; });
            if (frameNumber == SHUTDOWN)
            {
                break;
            }
        }
        frameNumberCV.notify_one();
        frameNumberIWantToProcess += 1;
        
        for(int i = threadId; i < meshes.Size(); i = i + threadCount)
        {
            EntityDescriptor entityId = meshes.MirrorToEntityDescriptor(i);

            TransformComponent faceTransform = transforms.Get(entityId.id);
            Matrix4 rX, rY, rZ, scaleMatrix, translationMatrix;
            scaleMatrix.scale(faceTransform.scale.x, faceTransform.scale.y, faceTransform.scale.z);
            rX.rotationX(faceTransform.rotation.x);
            rY.rotationY(faceTransform.rotation.y);
            rZ.rotationZ(faceTransform.rotation.z);
            translationMatrix.translation(faceTransform.position.x, faceTransform.position.y, faceTransform.position.z);

            Matrix4 localSpaceToWorldSpaceTransform;
            localSpaceToWorldSpaceTransform.identity();
            localSpaceToWorldSpaceTransform = translationMatrix * rZ * rY * rX * scaleMatrix;

            for (auto& f : meshLib[meshes.Get(entityId.id).meshAssetCode])
            {
                Face faceTransformed;
                for (int j = 0; j < 3; j++)
                {
                    faceTransformed.points[j] = localSpaceToWorldSpaceTransform * f.points[j];
                }

                Vector4 line1 = faceTransformed.points[1] - faceTransformed.points[0];
                Vector4 line2 = faceTransformed.points[2] - faceTransformed.points[0];
                Vector4 surfaceNormal = line1^line2;
                surfaceNormal.Normalize();
                Vector4 cameraToFaceRay = faceTransformed.points[0] - mainCamera.position;
                bool facingAwayFromCamera = (cameraToFaceRay * surfaceNormal) >= 0.0f;

                if (!facingAwayFromCamera)
                {
                    Face faceProjected;
                    bool tooCloseOrBehindCamera = false;
                    for (int j = 0; j < 3; j++)
                    {
                        faceProjected.points[j] = worldSpaceToClipSpaceTransform * faceTransformed.points[j];
                        if (faceProjected.points[j].w > zNear/2)
                        {
                            // This takes us from Clip space to NDC space
                            faceProjected.points[j] = faceProjected.points[j] * (1.0f / faceProjected.points[j].w);
                        }
                        else
                        {
                            tooCloseOrBehindCamera = true;
                            break;
                        }
                    }
                    if (tooCloseOrBehindCamera)
                    {
                        // TODO: This flagging system isn't pleasant to look at...
                        continue;
                    }

                    if (faceProjected.isWithinNDCCube())
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            // This takes us from NDC space to Screen space
                            faceProjected.points[k].x = (faceProjected.points[k].x + 1.0f) * 0.5f * SCREEN_WIDTH;
                            faceProjected.points[k].y = (faceProjected.points[k].y + 1.0f) * 0.5f * SCREEN_HEIGHT;
                        }


                        faceProjected.entityId = entityId.id;
                        parallelRenderQueues[threadId].push_back(faceProjected);
                    }
                }
            }
        }

        {
            std::lock_guard<std::mutex> numThreadsDoneLock(numThreadsDoneMutex);
            numThreadsDone++;
        }
        numThreadsDoneCV.notify_one();
    }

    // You only reach here if it's time to shutdown, so make sure any other
    // threads waiting know that the frameNumber has been updated
    frameNumberCV.notify_one();
}

void Renderer::Shutdown()
{
    {
        std::lock_guard<std::mutex> guard(frameNumberMutex);
        frameNumber = SHUTDOWN;
    }
    frameNumberCV.notify_one();

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void Renderer::SetCameraSpaceToClipSpaceTransform()
{
    const float pi = acosf(-1.0);   // cos(pi) = -1, so inverting gives us pi :D
    const float fovRad = fovDeg * ( pi / 180.0f );
    const float c = 1.0f / tanf(fovRad / 2.0f);
    cameraSpaceToClipSpaceTransform(0, 0) = c / aspectRatio;
    cameraSpaceToClipSpaceTransform(1, 1) = c;
    cameraSpaceToClipSpaceTransform(2, 2) = zFar / (zFar - zNear);
    cameraSpaceToClipSpaceTransform(2, 3) = -(zFar * zNear) / (zFar - zNear);
    cameraSpaceToClipSpaceTransform(3, 2) = 1.0f;
}

void Renderer::SetCameraMatrices()
{
    mainCamera.UpdateCameraFrame();

    /*
    We can directly compute the inverse but I'll leave this here for future reference. This is an affine tranform so it can be
    inverted according to some special properties that I don't have memorized :)
    cameraMatrix(0, 0) = right.x;   cameraMatrix(0, 1) = up.x;  cameraMatrix(0, 2) = forward.x;     cameraMatrix(0, 3) = camera.x;
    cameraMatrix(1, 0) = right.y;   cameraMatrix(1, 1) = up.y;  cameraMatrix(1, 2) = forward.y;     cameraMatrix(1, 3) = camera.y;
    cameraMatrix(2, 0) = right.z;   cameraMatrix(2, 1) = up.z;  cameraMatrix(2, 2) = forward.z;     cameraMatrix(2, 3) = camera.z;
    cameraMatrix(3, 0) = 0.0f;      cameraMatrix(3, 1) = 0.0f;  cameraMatrix(3, 2) = 0.0f;          cameraMatrix(3, 3) = 1.0f;
    */

    worldSpaceToCameraSpaceTransform(0, 0) = mainCamera.right.x;    worldSpaceToCameraSpaceTransform(0, 1) = mainCamera.right.y;    worldSpaceToCameraSpaceTransform(0, 2) = mainCamera.right.z;
    worldSpaceToCameraSpaceTransform(1, 0) = mainCamera.up.x;       worldSpaceToCameraSpaceTransform(1, 1) = mainCamera.up.y;       worldSpaceToCameraSpaceTransform(1, 2) = mainCamera.up.z;
    worldSpaceToCameraSpaceTransform(2, 0) = mainCamera.forward.x;  worldSpaceToCameraSpaceTransform(2, 1) = mainCamera.forward.y;  worldSpaceToCameraSpaceTransform(2, 2) = mainCamera.forward.z;
    worldSpaceToCameraSpaceTransform(3, 0) = 0.0f;       worldSpaceToCameraSpaceTransform(3, 1) = 0.0f;       worldSpaceToCameraSpaceTransform(3, 2) = 0.0f;       worldSpaceToCameraSpaceTransform(3, 3) = 1.0f;

    worldSpaceToCameraSpaceTransform(0, 3) = -(mainCamera.right.x   * mainCamera.position.x  +  mainCamera.right.y   * mainCamera.position.y  +  mainCamera.right.z   * mainCamera.position.z);
    worldSpaceToCameraSpaceTransform(1, 3) = -(mainCamera.up.x      * mainCamera.position.x  +  mainCamera.up.y      * mainCamera.position.y  +  mainCamera.up.z      * mainCamera.position.z);
    worldSpaceToCameraSpaceTransform(2, 3) = -(mainCamera.forward.x * mainCamera.position.x  +  mainCamera.forward.y * mainCamera.position.y  +  mainCamera.forward.z * mainCamera.position.z);
}
