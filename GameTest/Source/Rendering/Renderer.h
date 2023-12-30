#pragma once

#include "../ECS/ECS.h"
#include "Drawing.h"

class Renderer
{
private:
    // Screen info
    const float SCREEN_WIDTH = 1280.0f;
    const float SCREEN_HEIGHT = 720.0f;

    // Projection matrix details
    float fovDeg = 70.0;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;

    // Lighting
    Vec3 lightDirection = Vec3(0.0f, 0.0f, -1.0f);

    // Face buffers
    int defaultBufferSize = 1000000;
    std::vector<Face> facesToRender;

    // Clipping planes
    std::vector<Vec3> clippingPlanePoints;
    std::vector<Vec3> clippingPlaneNormals;

    // Multi-threading
    const int DEFAULT_THREAD_COUNT = 2; // Steam hardware survey says 0.06% of their users have a single-core cpu. 6+% have a dual-core
    int threadCount = (std::thread::hardware_concurrency() == 0) ? DEFAULT_THREAD_COUNT : std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // Used for thread coordination
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
    Vec3 cameraLookDirection = Vec3(0.0f, 0.0f, 1.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 right;
    Vec3 camera = Vec3(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;


    float theta = 0;
    Matrix4x4 cameraAndProjectionMatrix;
    Matrix4x4 projectionMatrix;
    Matrix4x4 cameraMatrix;
    Matrix4x4 inverseCameraMatrix;

    Renderer();
    void init(ECS& ecs);
    void Render();
    void parallelProcessMesh(int threadId);
    void shutdown();

    void setProjectionMatrix();
    void setCameraMatrices();
};