#pragma once

#include "../ECS/EntityDescriptor.h"

struct SinWaveAIComponent
{
    float SPEED = 0.1f;
    float theta = -acos(-1.0f);

    SinWaveAIComponent();
};
