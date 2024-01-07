#pragma once

#include "../Math/Vec3.h"

struct TransformComponent
{
    Vec3 v;

    TransformComponent();
    TransformComponent(float x, float y, float z);
};
