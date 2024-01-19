#include "stdafx.h"

#include "Camera.h"

Camera::Camera() {}

void Camera::UpdateCameraFrame()
{
    Matrix4 cameraRotation;
    cameraRotation.rotationY(yaw);

    Vector4 target = Vector4(0.0f, 0.0f, 1.0f);
    lookDirection = cameraRotation * target;
    target = position + lookDirection;

    forward = target - position;
    forward.Normalize();

    up = worldUp - (forward * (up * forward));
    up.Normalize();

    right = up ^ forward;
}
