#pragma once

#include "Drawing.h"
#include <thread>
#include <algorithm>

class Renderer
{
private:
    // Screen info
    const float SCREEN_WIDTH = 1024.0f;
    const float SCREEN_HEIGHT = 768.0f;

    // Projection matrix details
    float fovDeg = 70.0;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_HEIGHT / SCREEN_WIDTH;

    // Lighting
    Vec3 lightDirection = Vec3(0.0f, 0.0f, -1.0f);

    // Multithreading
    bool gameOver;
    int threadCountHint = std::thread::hardware_concurrency();  // This does not necessarily reflect a suitable number of threads to create
    int threadCount = (threadCountHint > 4) ? threadCountHint : 4;
    std::vector<std::thread> threads;

    // Face buffers
    int defaultBufferSize = 1000000;
    std::vector<std::vector<Face>> facesProcessedByThread;
    std::vector<Face> facesToRender;

public:
    // Camera
    Vec3 cameraLookDirection = Vec3(0.0f, 0.0f, 1.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 right;
    Vec3 camera = Vec3(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;


    float theta = 0;
    Matrix4x4 projectionMatrix;
    Matrix4x4 cameraMatrix;
    Matrix4x4 inverseCameraMatrix;
    Matrix4x4 cameraAndProjectionMatrix;

    Renderer();
    void Init();
    void Render(Pool<MeshComponent> &meshes, Pool<TransformComponent> &transforms, Pool<MeshResourceComponent> &meshResources);
    void calculateFace();
    void shutdown();
    void setProjectionMatrix();
    void setCameraMatrices();
};