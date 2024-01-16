#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() {}

void Renderer::init(ECS& ecs)
{
    //
    this->ecs = &ecs;

    // Define clipping planes
    // Bottom
    clippingPlanePoints.emplace_back(0.0f, -1.0f, 0.0f);
    clippingPlaneNormals.emplace_back(0.0f, 1.0f, 0.0f);

    // Top
    clippingPlanePoints.emplace_back(0.0f, 1.0f, 0.0f);
    clippingPlaneNormals.emplace_back(0.0f, -1.0f, 0.0f);

    // Left
    clippingPlanePoints.emplace_back(-1.0f, 0.0f, 0.0f);
    clippingPlaneNormals.emplace_back(1.0f, 0.0f, 0.0f);

    // Right
    clippingPlanePoints.emplace_back(1.0f, 0.0f, 0.0f);
    clippingPlaneNormals.emplace_back(-1.0f, 0.0f, 0.0f);

    // Near
    clippingPlanePoints.emplace_back(0.0f, 0.0f, 1.0f);
    clippingPlaneNormals.emplace_back(0.0f, 0.0f, -1.0f);

    // Other setup
    facesToRender.reserve(defaultBufferSize);
    lightDirection.Normalize();
    setProjectionMatrix();

    // Create threads
    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back(&Renderer::parallelProcessMesh, this, i);
        internalToThreadFacesToRender.emplace_back();
        internalToThreadFacesToRender[i].reserve(defaultBufferSize / threadCount);
    }
}

void Renderer::Render()
{
    Pool<TextureComponent>& textures = ecs->GetTextures();

    // These matrices are the same for each face in a frame so calculate them once now
    setCameraMatrices();
    cameraAndProjectionMatrix = projectionMatrix * inverseCameraMatrix;

    // Let worker threads know it's time to work on the next frame
    {
        std::lock_guard<std::mutex> frameNumberLock(frameNumberMutex);
        frameNumber++;
    }
    frameNumberCV.notify_one();

    // Wait for threads to finish
    {
        std::unique_lock<std::mutex> numThreadsDoneLock(numThreadsDoneMutex);
        numThreadsDoneCV.wait(numThreadsDoneLock, [this] { return numThreadsDone == threadCount; });
        numThreadsDone = 0;
    }

    // Merge each thread's internal face buffers
    for (int i = 0; i < threadCount; i++)
    {
        facesToRender.insert(facesToRender.end(), internalToThreadFacesToRender[i].begin(), internalToThreadFacesToRender[i].end());
    }

    // Painter's algorithm because I can't use a z-buffer :(
    std::sort(facesToRender.begin(), facesToRender.end());
    for (int i = 0; i < facesToRender.size(); i++)
    {
        if (textures.Get(facesToRender[i].entityId)->fill.r != -1.0f)
        {
            drawFilledTriangle(facesToRender[i], textures.Get(facesToRender[i].entityId)->fill);
        }
        drawTriangle(facesToRender[i], textures.Get(facesToRender[i].entityId)->outline);
    }

    // I don't want faces from previous frames to linger in the buffers
    facesToRender.clear();
    for (int i = 0; i < threadCount; i++)
    {
        internalToThreadFacesToRender[i].clear();
    }
}

void Renderer::parallelProcessMesh(int threadId)
{
    long frameNumberIWantToProcess = 1;

    Pool<MeshComponent>& meshes = ecs->GetMeshes();
    Pool<MeshResourceComponent>& meshResources = ecs->GetMeshResources();
    Pool<TransformComponent>& transforms = ecs->GetTransforms();

    while (true)
    {
        {
            std::unique_lock<std::mutex> frameNumberLock(frameNumberMutex);
            frameNumberCV.wait(frameNumberLock, [this, frameNumberIWantToProcess] { return frameNumber == frameNumberIWantToProcess || frameNumber == -1; });
            if (frameNumber == -1)
            {
                break;
            }
        }
        frameNumberCV.notify_one();
        frameNumberIWantToProcess += 1;
        
        for(int i = threadId; i < meshes.Size(); i = i + threadCount)
        {
            int meshResourceId = meshes._dense[i].meshResourceId;
            int entityId = meshes.MirrorIdToEntityId(i);

            Matrix4 rX, rY, rZ, translationMatrix;

            rX.rotationX(theta);
            rY.rotationY(theta * 0.5f);
            rZ.rotationZ(theta * 0.25f);

            Vector4 position = transforms.Get(entityId)->v;
            translationMatrix.translation(position.x, position.y, position.z);

            Matrix4 worldMatrix;
            worldMatrix.identity();
            worldMatrix = translationMatrix * rX * rY * rZ;

            for (auto& f : meshResources.Get(meshResourceId)->faces)
            {
                Face faceTransformed;
                for (int j = 0; j < 3; j++)
                {
                    faceTransformed.points[j] = worldMatrix * f.points[j];
                }

                Vector4 line1 = faceTransformed.points[1] - faceTransformed.points[0];
                Vector4 line2 = faceTransformed.points[2] - faceTransformed.points[0];
                Vector4 faceNormal = line1^line2;
                faceNormal.Normalize();

                float faceNormalProjectionOntoCameraRay = (faceTransformed.points[0] - camera) * faceNormal;
                if (faceNormalProjectionOntoCameraRay < 0.0f)
                {
                    Face faceProjected;
                    bool tooCloseOrBehindCamera = false;
                    for (int j = 0; j < 3; j++)
                    {
                        faceProjected.points[j] = cameraAndProjectionMatrix * faceTransformed.points[j];
                        if (faceProjected.points[j].w > zNear)
                        {
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
                        // TODO: This flagging system isn't pleasant to look at.
                        continue;
                    }

                    if (faceProjected.isWithinNDCCube())
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            faceProjected.points[k].x = (faceProjected.points[k].x + 1.0f) * 0.5f * SCREEN_WIDTH;
                            faceProjected.points[k].y = (faceProjected.points[k].y + 1.0f) * 0.5f * SCREEN_HEIGHT;
                        }


                        faceProjected.entityId = entityId;
                        internalToThreadFacesToRender[threadId].push_back(faceProjected);
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

    // You only reach here if it's time to shutdown (frame number == -1), so make sure any other
    // threads waiting know that the frameNumber has been updated (to -1) so they can also exit
    // the while loop and join.
    frameNumberCV.notify_one();
}

void Renderer::shutdown()
{
    {
        std::lock_guard<std::mutex> guard(frameNumberMutex);
        frameNumber = -1;
    }
    frameNumberCV.notify_one();

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void Renderer::setProjectionMatrix()
{
    const float pi = acosf(-1.0);   // cos(pi) = -1, so inverting gives us pi :D
    const float fovRad = fovDeg * ( pi / 180.0f );
    const float c = 1.0f / tanf(fovRad / 2.0f);
    projectionMatrix(0, 0) = c / aspectRatio;
    projectionMatrix(1, 1) = c;
    projectionMatrix(2, 2) = zFar / (zFar - zNear);
    projectionMatrix(2, 3) = -(zFar * zNear) / (zFar - zNear);
    projectionMatrix(3, 2) = 1.0f;
}

void Renderer::setCameraMatrices()
{
    Vector4 cameraTarget = Vector4(0.0f, 0.0f, 1.0f);
    Matrix4 cameraRotation;
    cameraRotation.rotationY(yaw);
    cameraLookDirection = cameraRotation * cameraTarget;
    cameraTarget = camera + cameraLookDirection;

    Vector4 forward = cameraTarget - camera;
    forward.Normalize();

    up = up - (forward * (up * forward));
    up.Normalize();

    right = up ^ forward;

    /*
    We can directly compute the inverse but I'll leave this here for future reference. This is an affine tranform so it can be
    inverted according to some special properties that I don't have memorized :)
    cameraMatrix(0, 0) = right.x;   cameraMatrix(0, 1) = up.x;  cameraMatrix(0, 2) = forward.x;     cameraMatrix(0, 3) = camera.x;
    cameraMatrix(1, 0) = right.y;   cameraMatrix(1, 1) = up.y;  cameraMatrix(1, 2) = forward.y;     cameraMatrix(1, 3) = camera.y;
    cameraMatrix(2, 0) = right.z;   cameraMatrix(2, 1) = up.z;  cameraMatrix(2, 2) = forward.z;     cameraMatrix(2, 3) = camera.z;
    cameraMatrix(3, 0) = 0.0f;      cameraMatrix(3, 1) = 0.0f;  cameraMatrix(3, 2) = 0.0f;          cameraMatrix(3, 3) = 1.0f;
    */

    inverseCameraMatrix(0, 0) = right.x;    inverseCameraMatrix(0, 1) = right.y;    inverseCameraMatrix(0, 2) = right.z;
    inverseCameraMatrix(1, 0) = up.x;       inverseCameraMatrix(1, 1) = up.y;       inverseCameraMatrix(1, 2) = up.z;
    inverseCameraMatrix(2, 0) = forward.x;  inverseCameraMatrix(2, 1) = forward.y;  inverseCameraMatrix(2, 2) = forward.z;
    inverseCameraMatrix(3, 0) = 0.0f;       inverseCameraMatrix(3, 1) = 0.0f;       inverseCameraMatrix(3, 2) = 0.0f;       inverseCameraMatrix(3, 3) = 1.0f;

    inverseCameraMatrix(0, 3) = -(right.x * camera.x +      right.y * camera.y +        right.z * camera.z);
    inverseCameraMatrix(1, 3) = -(up.x * camera.x +         up.y * camera.y +           up.z * camera.z);
    inverseCameraMatrix(2, 3) = -(forward.x * camera.x +    forward.y  * camera.y +     forward.z * camera.z);
}
