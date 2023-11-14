#pragma once

#include "../Util/GameMath.h"

struct TransformComponent
{
    Vec3 v;

    TransformComponent();
    TransformComponent(float x, float y, float z);
};
