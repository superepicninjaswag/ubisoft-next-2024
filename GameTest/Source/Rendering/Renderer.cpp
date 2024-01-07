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
        internalToThreadFacesToRender.reserve(defaultBufferSize / threadCount);
    }
}

void Renderer::Render()
{
    Pool<TextureComponent>& textures = ecs->GetTextures();

    // These matrices are the same for each face in a frame so calculate them once now
    setCameraMatrices();
    cameraAndProjectionMatrix = inverseCameraMatrix * projectionMatrix;

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

            Matrix4x4 rX, rY, rZ, translationMatrix;

            rX.rotationX(theta);
            rY.rotationY(theta * 0.5f);
            rZ.rotationZ(theta * 0.25f);

            Vec3 position = transforms.Get(entityId)->v;
            translationMatrix.translation(position.x, position.y, position.z);

            Matrix4x4 worldMatrix = rX * rY * rZ * translationMatrix;

            for (auto& f : meshResources.Get(meshResourceId)->faces)
            {
                Face faceTransformed;
                for (int j = 0; j < 3; j++)
                {
                    faceTransformed.points[j] = worldMatrix * f.points[j];
                }

                // Calculate face normal
                Vec3 line1 = faceTransformed.points[1] - faceTransformed.points[0];
                Vec3 line2 = faceTransformed.points[2] - faceTransformed.points[0];
                Vec3 normal = line1.CrossProduct(line2);
                normal.Normalize();

                // Normal projection onto line from camera to face
                float faceNormalProjectionOntoCameraRay = normal * (faceTransformed.points[0] - camera);

                /*
                If the projection of the face normal is negative (relative
                to a ray from the camera), then we know the face is pointing
                away from the camera
                */
                if (faceNormalProjectionOntoCameraRay < 0.0f)
                {
                    // Project the face from world space to camera space to normalized
                    Face faceProjected;
                    for (int j = 0; j < 3; j++)
                    {
                        faceProjected.points[j] = cameraAndProjectionMatrix * faceTransformed.points[j];
                        faceProjected.points[j] = faceProjected.points[j] * (1.0f / faceProjected.points[j].w);
                    }


                    // Clip triangle
                    /*std::list<Face> clippedTriangles;
                    clippedTriangles.push_back(faceProjected);
                    
                    for (int j = 0; j < clippingPlanePoints.size(); j++)
                    {
                        int numTriangles = clippedTriangles.size();
                        int k = 0;
                        while (k < numTriangles)
                        {
                            Face f = clippedTriangles.front();
                            clippedTriangles.pop_front();
                            std::vector<Face> newClippedTriangles = f.clipAgainstPlane(clippingPlanePoints[j], clippingPlaneNormals[j]);
                            clippedTriangles.insert(clippedTriangles.end(), newClippedTriangles.begin(), newClippedTriangles.end());
                            k++;
                        }
                    }

                    for (int j = 0; j < clippedTriangles.size(); j++)
                    {
                        Face f = clippedTriangles.front();
                        clippedTriangles.pop_front();*/
                    // Scale from normalized to screen size
                    for (int k = 0; k < 3; k++)
                    {
                        faceProjected.points[k].x = (faceProjected.points[k].x + 1.0f) * 0.5f * SCREEN_WIDTH;
                        faceProjected.points[k].y = (faceProjected.points[k].y + 1.0f) * 0.5f * SCREEN_HEIGHT;
                    }

                    // Lighting and colour data transfer to face
                    /*if (meshes._dense[i].isFilled)
                    {
                        float dp = normal * lightDirection;
                        faceProjected.fillColour.r = dp * meshes._dense[i].fillColour.r;
                        faceProjected.fillColour.g = dp * meshes._dense[i].fillColour.g;
                        faceProjected.fillColour.b = dp * meshes._dense[i].fillColour.b;
                        faceProjected.isFilled = true;
                    }
                    faceProjected.outlineColour = meshes._dense[i].outlineColour;
                    */

                    // Add face to render queue
                    faceProjected.entityId = entityId;
                    internalToThreadFacesToRender[threadId].push_back(faceProjected);
                    //}
                }
            }
        }

        // How else will the main thread know everything is complete!
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
    // Matrix definition taken from Real-time rendering textbook
    float pi = 3.14159265f;
    float fovRad = 1.0f / tanf(fovDeg / 2.0f / 180.0f * pi);
    projectionMatrix(0, 0) = fovRad / aspectRatio;
    projectionMatrix(1, 1) = fovRad;
    projectionMatrix(2, 2) = (zFar + zNear) / (zFar - zNear);
    projectionMatrix(2, 3) = 1.0f;
    projectionMatrix(3, 2) = -(zFar * zNear) / (zFar - zNear);
}

void Renderer::setCameraMatrices()
{
    Vec3 cameraTarget = Vec3(0.0f, 0.0f, 1.0f);
    Matrix4x4 cameraRotation;
    cameraRotation.rotationY(yaw);
    cameraLookDirection = cameraRotation * cameraTarget;
    cameraTarget = camera + cameraLookDirection;

    Vec3 forward = cameraTarget - camera;
    forward.Normalize();

    up = up - (forward * (up * forward));
    up.Normalize();

    right = up.CrossProduct(forward);

    cameraMatrix(0, 0) = right.x;    cameraMatrix(0, 1) = right.y;    cameraMatrix(0, 2) = right.z;    cameraMatrix(0, 3) = 0.0f;
    cameraMatrix(1, 0) = up.x;       cameraMatrix(1, 1) = up.y;       cameraMatrix(1, 2) = up.z;       cameraMatrix(1, 3) = 0.0f;
    cameraMatrix(2, 0) = forward.x;  cameraMatrix(2, 1) = forward.y;  cameraMatrix(2, 2) = forward.z;  cameraMatrix(2, 3) = 0.0f;
    cameraMatrix(3, 0) = camera.x;   cameraMatrix(3, 1) = camera.y;   cameraMatrix(3, 2) = camera.z;   cameraMatrix(3, 3) = 1.0f;

    inverseCameraMatrix(0, 0) = cameraMatrix(0, 0); inverseCameraMatrix(0, 1) = cameraMatrix(1, 0); inverseCameraMatrix(0, 2) = cameraMatrix(2, 0); inverseCameraMatrix(0, 3) = 0.0f;
    inverseCameraMatrix(1, 0) = cameraMatrix(0, 1); inverseCameraMatrix(1, 1) = cameraMatrix(1, 1); inverseCameraMatrix(1, 2) = cameraMatrix(2, 1); inverseCameraMatrix(1, 3) = 0.0f;
    inverseCameraMatrix(2, 0) = cameraMatrix(0, 2); inverseCameraMatrix(2, 1) = cameraMatrix(1, 2); inverseCameraMatrix(2, 2) = cameraMatrix(2, 2); inverseCameraMatrix(2, 3) = 0.0f;
    inverseCameraMatrix(3, 0) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 0) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 0) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 0));
    inverseCameraMatrix(3, 1) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 1) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 1) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 1));
    inverseCameraMatrix(3, 2) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 2) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 2) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 2));
    inverseCameraMatrix(3, 3) = 1.0f;
}
