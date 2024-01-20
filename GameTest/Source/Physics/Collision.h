#pragma once

#include "../ECS/EntityDescriptor.h"

struct Collision
{
	EntityDescriptor m_entityA;
	EntityDescriptor m_entityB;

	Collision(const EntityDescriptor entityA, const EntityDescriptor entityB);
};
