#pragma once

#include "../Math/Vector4.h"

struct ParticleComponent
{
    Vector4 velocity;
    int remainingLifetimeFrames = 0;

    ParticleComponent (float x, float y, float z, int lifetime);
};