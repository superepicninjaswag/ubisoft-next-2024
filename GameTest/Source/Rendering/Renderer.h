#pragma once

#include "../ECS/ECS.h"
#include "MeshLibrary.h"
#include "Drawing.h"

class Renderer
{
private:
    ECS &ecs;
    MeshLibrary &meshLib;

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
    Vector4 cameraLookDirection = Vector4(0.0f, 0.0f, 1.0f);
    Vector4 up = Vector4(0.0f, 1.0f, 0.0f);
    Vector4 right;
    Vector4 camera = Vector4(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;


    Matrix4 worldSpaceToClipSpaceTransform;
    Matrix4 cameraSpaceToClipSpaceTransform;
    Matrix4 cameraMatrix;
    Matrix4 worldSpaceToCameraSpaceTransform;

    Renderer(ECS& ecsReference , MeshLibrary& meshLibraryReference);
    void Init();
    void Render();
    void ParallelProcessMesh(int threadId);
    void Shutdown();

    void SetCameraSpaceToClipSpaceTransform();  // Projection Matrix
    void SetCameraMatrices();
};