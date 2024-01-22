#pragma once

#include "../ECS/EntityDescriptor.h"

struct DodgingAIComponent
{
    enum States
    {
        ATTACK,
        DODGE
    };

    int currentState = States::ATTACK;

    DodgingAIComponent();
};
