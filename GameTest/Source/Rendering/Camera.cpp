#include "stdafx.h"

#include "Camera.h"

Camera::Camera()
{
    App::GetMousePos(currentMouseX, currentMouseY);
}

void Camera::UpdateCameraFrame()
{
    Matrix4 yawRotation;
    yawRotation.rotationY(m_yaw);
    Matrix4 pitchRotation;
    pitchRotation.rotationX(m_pitch);

    Vector4 target = Vector4(0.0f, 0.0f, 1.0f);
    m_lookDirection = yawRotation * (pitchRotation * target);
    target = m_position + m_lookDirection;

    forward = target - m_position;
    forward.Normalize();

    right = m_worldUp ^ forward;
    right.Normalize();

    up = forward ^ right;
    up.Normalize();
}

void Camera::UpdatePosition(Vector4 position)
{
    m_position = position;
}

void Camera::UpdatePitchAndYaw(float deltaTime)
{
    float borderWidth = 100.0f;
    App::GetMousePos(currentMouseX, currentMouseY);
    if (currentMouseX < borderWidth)
    {
        m_yaw -= RADIANS_PER_SECOND;
    }
    else if (currentMouseX > APP_VIRTUAL_WIDTH - borderWidth)
    {
        m_yaw += RADIANS_PER_SECOND;
    }
    if (currentMouseY < borderWidth)
    {
        m_pitch += RADIANS_PER_SECOND;
    }
    else if (currentMouseY > APP_VIRTUAL_HEIGHT - borderWidth)
    {
        m_pitch -= RADIANS_PER_SECOND;
    }

    // Looking directly up or down mangles the camera frame. This prevents that
    m_pitch = min(m_pitch, MAX_PITCH);
    m_pitch = max(-MAX_PITCH, m_pitch);
}