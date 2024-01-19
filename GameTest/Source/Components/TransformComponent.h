#pragma once

#include "../Math/Vector4.h"

struct TransformComponent
{
    Vector4 position;
    Vector4 scale;
    Vector4 rotation;

    TransformComponent();
    TransformComponent(float x, float y, float z);
};
