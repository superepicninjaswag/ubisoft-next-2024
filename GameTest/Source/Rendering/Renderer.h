#pragma once

#include "../ECS/ECS.h"
#include "Drawing.h"

class Renderer
{
private:
    // Screen info
    const float SCREEN_WIDTH = 1024;
    const float SCREEN_HEIGHT = 768;

    // Projection matrix details
    float fovDeg = 70.0;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;

    // Lighting
    Vector4 lightDirection = Vector4(0.0f, 0.0f, -1.0f);

    // Face buffers
    int defaultBufferSize = 1000000;
    std::vector<Face> facesToRender;
    std::vector<std::vector<Face>> internalToThreadFacesToRender;

    // Clipping planes
    std::vector<Vector4> clippingPlanePoints;
    std::vector<Vector4> clippingPlaneNormals;

    // Multi-threading
    const int DEFAULT_THREAD_COUNT = 2; // Steam hardware survey says 0.06% of their users have a single-core cpu. 6+% have a dual-core
    const int threadCount = (std::thread::hardware_concurrency() == 0) ? DEFAULT_THREAD_COUNT : std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // Thread coordination
    int numThreadsDone = 0;
    long frameNumber = 0;

    std::mutex frameNumberMutex;
    std::condition_variable frameNumberCV;

    std::mutex numThreadsDoneMutex;
    std::condition_variable numThreadsDoneCV;

    std::mutex facesToRenderMutex;

    // ECS
    ECS* ecs;

public:
    // Camera
    Vector4 cameraLookDirection = Vector4(0.0f, 0.0f, 1.0f);
    Vector4 up = Vector4(0.0f, 1.0f, 0.0f);
    Vector4 right;
    Vector4 camera = Vector4(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;


    float theta = 0;
    Matrix4 cameraAndProjectionMatrix;
    Matrix4 projectionMatrix;
    Matrix4 cameraMatrix;
    Matrix4 inverseCameraMatrix;

    Renderer();
    void init(ECS& ecs);
    void Render();
    void parallelProcessMesh(int threadId);
    void shutdown();

    void setProjectionMatrix();
    void setCameraMatrices();
};