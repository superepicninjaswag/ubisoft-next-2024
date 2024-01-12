#pragma once

#include "../Math/Vector4.h"

struct TransformComponent
{
    Vector4 v;

    TransformComponent();
    TransformComponent(float x, float y, float z);
};
