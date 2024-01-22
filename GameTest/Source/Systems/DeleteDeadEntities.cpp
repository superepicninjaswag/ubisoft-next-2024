#include "stdafx.h"

#include "Systems.h"

void DeleteDeadEntities(ECS& ecs)
{
	ComponentPool<HealthComponent>& health = ecs.GetHealth();
	std::vector<EntityDescriptor> victims;
	for (int i = 0; i < health.Size(); i++)
	{
		EntityDescriptor target = health.MirrorToEntityDescriptor(i);
		if (health.Get(target.id).m_health < 1)
		{
			victims.push_back(target);
		}
	}

	for (int i = 0; i < victims.size(); i++)
	{
		ecs.DeleteEntity(victims[i]);
	}
}