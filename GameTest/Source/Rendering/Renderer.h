#pragma once

#include "../ECS/ECS.h"
#include "MeshLibrary.h"
#include "Drawing.h"
#include "Camera.h"

class Renderer
{
private:
    ECS &ecs;
    MeshLibrary &meshLib;
    Camera &camera;

    const float SCREEN_WIDTH = APP_VIRTUAL_WIDTH;
    const float SCREEN_HEIGHT = APP_VIRTUAL_HEIGHT;

    float fovDeg = 90.0;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;

    int DEFAULT_BUFFER_SIZE = 1000000;
    std::vector<Face> renderQueue;
    std::vector<std::vector<Face>> parallelRenderQueues;

    const int DEFAULT_THREAD_COUNT = 2; // Steam hardware survey says 0.06% of their users have a single-core cpu. 6+% have a dual-core
    const int threadCount = (std::thread::hardware_concurrency() == 0) ? DEFAULT_THREAD_COUNT : std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int numThreadsDone = 0;
    long frameNumber = 0;
    const long SHUTDOWN = -1;

    std::mutex frameNumberMutex;
    std::condition_variable frameNumberCV;

    std::mutex numThreadsDoneMutex;
    std::condition_variable numThreadsDoneCV;

public:
    Matrix4 worldSpaceToClipSpaceTransform;
    Matrix4 cameraSpaceToClipSpaceTransform;
    Matrix4 cameraMatrix;
    Matrix4 worldSpaceToCameraSpaceTransform;

    Renderer(ECS& ecsReference , MeshLibrary& meshLibraryReference, Camera& cameraReference);
    void Init();
    void Render();
    void ParallelProcessMesh(int threadId);
    void Shutdown();

    void SetCameraSpaceToClipSpaceTransform();  // Projection Matrix
    void SetCameraMatrices();

    float GetFOV();
    float GetAspectRatio();
};