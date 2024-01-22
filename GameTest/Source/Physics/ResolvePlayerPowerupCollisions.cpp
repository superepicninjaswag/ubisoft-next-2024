#include "stdafx.h"

#include "ResolvePlayerPowerupCollisions.h"

void ResolvePlayerPowerupCollisions(ECS& ecs, Ground& ground, Gun& gun, EntityDescriptor player)
{
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
	ComponentPool<PowerupComponent>& powerups = ecs.GetPowerups();

	std::vector<EntityDescriptor> needToBeDeleted;
	for (int i = 0; i < powerups.Size(); i++)
	{
		EntityDescriptor powerup = powerups.MirrorToEntityDescriptor(i);

		SphereColliderComponent colliderPowerup = spheres.Get(powerup.id);
		SphereColliderComponent colliderPlayer = spheres.Get(player.id);

		TransformComponent transformPowerup = transforms.Get(powerup.id);
		TransformComponent transformPlayer = transforms.Get(player.id);

		// From 4.3.1 in "Real-Time Collision Detection" by Christer Ericson
		Vector4 d = transformPowerup.position - transformPlayer.position;
		float dist2 = d * d;
		float radiusSum = colliderPowerup.m_radius + colliderPlayer.m_radius;
		if (dist2 <= radiusSum * radiusSum)
		{
			needToBeDeleted.push_back(powerup);
			gun.AddSupergunTime(powerups.Get(powerup.id).m_timeProvided);
		}
	}

	for (int i = 0; i < needToBeDeleted.size(); i++)
	{
		ecs.DeleteEntity(needToBeDeleted[i]);
	}
}
