#pragma once

#include "../ECS/EntityDescriptor.h"

struct Contact
{
	EntityDescriptor m_entityA;
	EntityDescriptor m_entityB;

	Contact(const EntityDescriptor entityA, const EntityDescriptor entityB);
};
