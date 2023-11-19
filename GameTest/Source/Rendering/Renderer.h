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

    // Camera
    Vec3 cameraLookDirection = Vec3(0.0f, 0.0f, 1.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

    // Lighting
    Vec3 lightDirection = Vec3(0.0f, 0.0f, -1.0f);

public:
    Vec3 camera = Vec3(0.0f, 0.0f, 0.0f);
    float theta = 0;
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewportMatrix;
    Matrix4x4 cameraMatrix;
    Matrix4x4 inverseCameraMatrix;

    Renderer();
    void Init();
    void Render(Pool<MeshComponent> &meshes, Pool<TransformComponent> &transforms, Pool<MeshResourceComponent> &meshResources);
    void setProjectionMatrix();
    void setViewportMatrix();
    void setCameraMatrices();
};