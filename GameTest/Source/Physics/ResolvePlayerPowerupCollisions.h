#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include "../Scenery/Ground.h"
#include "../GameObjects/Gun.h"

void ResolvePlayerPowerupCollisions(ECS& ecs, Ground& ground, Gun& gun, EntityDescriptor player);