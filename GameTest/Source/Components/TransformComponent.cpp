#include "stdafx.h"

#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;

    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;

    rotation.x = 0.0f;
    rotation.y = 0.0f;
    rotation.z = 0.0f;
}

TransformComponent::TransformComponent(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;

    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;

    rotation.x = 0.0f;
    rotation.y = 0.0f;
    rotation.z = 0.0f;
}