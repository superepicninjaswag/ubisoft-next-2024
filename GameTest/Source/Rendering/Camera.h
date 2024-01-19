#pragma once

#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"


class Camera
{
public:
    Vector4 lookDirection = Vector4(0.0f, 0.0f, 1.0f);
    Vector4 worldUp = Vector4(0.0f, 1.0f, 0.0f);
    Vector4 position = Vector4(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;

    Vector4 forward, right, up;

    Camera();
    void UpdateCameraFrame();
};