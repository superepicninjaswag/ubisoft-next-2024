#pragma once

#include "../ECS/ECS.h"
#include "../Components/Components.h"
#include "Contact.h"
#include "../Scenery/Ground.h"

void ResolveBulletGroundCollisions(ECS& ecs, Ground& ground);