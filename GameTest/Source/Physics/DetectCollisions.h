#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include "Collision.h"

void DetectCollisions(ECS& ecs, std::vector<Collision>& collisionQueue);