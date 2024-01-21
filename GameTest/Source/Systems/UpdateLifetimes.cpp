#include "stdafx.h"

#include "Systems.h"

void UpdateLifetimes(ECS& ecs)
{
	ComponentPool<LifetimeComponent>& lifetimes = ecs.GetLifetimes();
	std::vector<EntityDescriptor> victims;
	for (int i = 0; i < lifetimes.Size(); i++)
	{
		EntityDescriptor target = lifetimes.MirrorToEntityDescriptor(i);
		if (lifetimes.Get(target.id).m_remainingLifetimeFrames < 1)
		{
			victims.push_back(target);
		}
		else
		{
			lifetimes.Get(target.id).m_remainingLifetimeFrames--;
		}
	}

	for (int i = 0; i < victims.size(); i++)
	{
		ecs.DeleteEntity(victims[i]);
	}
}
