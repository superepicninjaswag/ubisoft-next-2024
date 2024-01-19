#pragma once

#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"


class Camera
{
public:
    float speed = 40.0f;
    const float SENSITIVITY = 2.0f;
    const float RADIANS_PER_PIXEL = 0.003;
    float currentMouseX;
    float currentMouseY;

    Vector4 lookDirection = Vector4(0.0f, 0.0f, 1.0f);
    Vector4 worldUp = Vector4(0.0f, 1.0f, 0.0f);
    Vector4 position = Vector4(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;

    Vector4 forward, right, up;

    Camera();
    void UpdateCameraFrame();
    void UpdatePosition(float deltaTime);
    void UpdatePitchAndYaw();
};