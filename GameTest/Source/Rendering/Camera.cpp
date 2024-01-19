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
        position = position + forwardFlattened * deltaTime * speed;
    }
    if (App::IsKeyPressed('S'))
    {
        position = position - forwardFlattened * deltaTime * speed;
    }
    if (App::IsKeyPressed('A'))
    {
        position = position - rightFlattened * deltaTime * speed;
    }
    if (App::IsKeyPressed('D'))
    {
        position = position + rightFlattened * deltaTime * speed;
    }
}

void Camera::UpdatePitchAndYaw()
{
    float oldMouseX = currentMouseX;
    float oldMouseY = currentMouseY;
    App::GetMousePos(currentMouseX, currentMouseY);
    if (oldMouseX != currentMouseX)
    {
        float deltaX = currentMouseX - oldMouseX;
        yaw += deltaX * SENSITIVITY * RADIANS_PER_PIXEL;
    }
    if (oldMouseY != currentMouseY)
    {
        float deltaY = currentMouseY - oldMouseY;
        pitch -= deltaY * SENSITIVITY * RADIANS_PER_PIXEL;

        // Looking exactly straight up or down causes problems so prevent that
        float maxPitch = 89.0f * (PI/180.0f);
        pitch = max(-maxPitch, min(pitch, maxPitch));
    }
}