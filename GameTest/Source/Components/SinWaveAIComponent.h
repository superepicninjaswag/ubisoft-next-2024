#pragma once

#include "../ECS/EntityDescriptor.h"

struct SinWaveAIComponent
{
    float SPEED = 0.5f;
    float theta = -acos(-1.0f);

    SinWaveAIComponent();
};
