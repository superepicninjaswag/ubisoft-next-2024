#pragma once

#include "../Components/Components.h"
#include "../ECS/ECS.h"
#include "../ECS/EntityDescriptor.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"
#include "../Scenery/Ground.h"
#include "../GameObjects/EntityBuilder.h"


void AnimateParticles(ECS& ecs);
void AnimateSinWaveAI(ECS& ecs, EntityDescriptor player);
void DeleteDeadEntities(ECS& ecs);
void EnemySpawner(ECS& ecs, float deltaTime, Ground ground);
void MovePlayer(ECS& ecs, EntityDescriptor player, Vector4 forward, Vector4 right, float speed, float deltaTime, Ground& ground);
void PowerupSpawner(ECS& ecs, Ground ground);
void UpdateLifetimes(ECS& ecs);