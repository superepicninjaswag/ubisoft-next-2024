#pragma once

#include "Rendering.h"
#include <thread>
#include <algorithm>

class Renderer
{
private:
    // Screen info
    const float SCREEN_HEIGHT = 768.0f;
    const float SCREEN_WIDTH = 1024.0f;

    // Projection matrix details
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float fovDeg = 70.0;
    float fovRad = 1.0 / tanf(fovDeg * 0.5f / 180.0f * pi);
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float aspectRatio = SCREEN_HEIGHT / SCREEN_WIDTH;

    // Etc
    Vec3 camera;
    Vec3 lightDirection;
    std::vector<std::thread> workers;

public:
    float theta = 0;
    Matrix4x4 matRotZ, matRotX;

    Renderer();
    void Init();
    void Render(Pool<MeshComponent> &meshes, Pool<TransformComponent> &transforms, Pool<MeshResourceComponent> &meshResources);
};