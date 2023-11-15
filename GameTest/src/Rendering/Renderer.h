#pragma once

#include "Drawing.h"
#include <thread>
#include <algorithm>

class Renderer
{
private:
    // Screen info
    const float SCREEN_HEIGHT = 768.0f;
    const float SCREEN_WIDTH = 1024.0f;

    // Projection matrix details
    float fovDeg = 90.0;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_HEIGHT / SCREEN_WIDTH;

    // Etc
    Vec3 camera;
    Vec3 lightDirection;
    std::vector<std::thread> workers;

public:
    float theta = 0;
    Matrix4x4 projectionMatrix;

    Renderer();
    void Init();
    void Render(Pool<MeshComponent> &meshes, Pool<TransformComponent> &transforms, Pool<MeshResourceComponent> &meshResources);
    void setProjectMatrix(float fovDeg, float aspectRatio, float zNear, float zFar);
};