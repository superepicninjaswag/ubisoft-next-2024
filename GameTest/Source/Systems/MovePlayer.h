#pragma once

#include "../Math/Vector4.h"
#include "../ECS/EntityDescriptor.h"
#include "../ECS/ECS.h"
#include "../Scenery/Ground.h"

void MovePlayer(ECS& ecs, EntityDescriptor player, Vector4 forward, Vector4 right, float speed, float deltaTime, Ground& ground);