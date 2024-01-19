#include "stdafx.h"

#include "Camera.h"

Camera::Camera()
{
    App::GetMousePos(currentMouseX, currentMouseY);
}

void Camera::UpdateCameraFrame()
{
    Matrix4 yawRotation;
    yawRotation.rotationY(yaw);
    Matrix4 pitchRotation;
    pitchRotation.rotationX(pitch);

    Vector4 target = Vector4(0.0f, 0.0f, 1.0f);
    lookDirection = yawRotation * (pitchRotation * target);
    target = position + lookDirection;

    forward = target - position;
    forward.Normalize();

    right = worldUp ^ forward;
    right.Normalize();

    up = forward ^ right;
    up.Normalize();
}

void Camera::UpdatePosition(float deltaTime)
{
    Vector4 forwardFlattened = lookDirection;
    forwardFlattened.y = 0.0f;
    forwardFlattened.Normalize();

    Vector4 rightFlattened = right;
    rightFlattened.y = 0.0f;
    rightFlattened.Normalize();

    if (App::IsKeyPressed('W'))
    {
        position = position + forwardFlattened * deltaTime * SPEED;
    }
    if (App::IsKeyPressed('S'))
    {
        position = position - forwardFlattened * deltaTime * SPEED;
    }
    if (App::IsKeyPressed('A'))
    {
        position = position - rightFlattened * deltaTime * SPEED;
    }
    if (App::IsKeyPressed('D'))
    {
        position = position + rightFlattened * deltaTime * SPEED;
    }
}

void Camera::UpdatePitchAndYaw(float deltaTime)
{
    float borderWidth = 100.0f;
    App::GetMousePos(currentMouseX, currentMouseY);
    if (currentMouseX < borderWidth)
    {
        yaw -= RADIANS_PER_SECOND;
    }
    else if (currentMouseX > APP_VIRTUAL_WIDTH - borderWidth)
    {
        yaw += RADIANS_PER_SECOND;
    }
    if (currentMouseY < borderWidth)
    {
        pitch += RADIANS_PER_SECOND;
    }
    else if (currentMouseY > APP_VIRTUAL_HEIGHT - borderWidth)
    {
        pitch -= RADIANS_PER_SECOND;
    }

    // Looking directly up or down mangles the camera frame. This prevents that
    pitch = min(pitch, MAX_PITCH);
    pitch = max(-MAX_PITCH, pitch);
}