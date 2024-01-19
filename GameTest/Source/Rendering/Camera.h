#pragma once

#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"


class Camera
{
public:
    const float SPEED = 40.0f;
    const float MAX_PITCH = 89.0f * (acosf(-1.0) / 180.0f); // cos(pi) = -1, so inverting gives us pi :D
    const float RADIANS_PER_SECOND = 0.02;
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
    void UpdatePitchAndYaw(float deltaTime);
};