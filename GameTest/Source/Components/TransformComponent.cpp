#include "stdafx.h"

#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
}

TransformComponent::TransformComponent(float x, float y, float z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}