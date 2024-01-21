#include "stdafx.h"

#include "ResolveBulletGroundCollisions.h"

void ResolveBulletGroundCollisions(ECS& ecs, Ground& ground)
{
	ComponentPool<ProjectileComponent>& projectiles = ecs.GetProjectiles();
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

	std::vector<EntityDescriptor> needToBeDeleted;
	for (int i = 0; i < projectiles.Size(); i++)
	{
		EntityDescriptor collider = projectiles.MirrorToEntityDescriptor(i);
		if (spheres.Has(collider) && transforms.Has(collider))
		{
			SphereColliderComponent sphere = spheres.Get(collider.id);
			TransformComponent transform = transforms.Get(collider.id);

			if (transform.position.y - sphere.m_radius < 0.0f)
			{
				needToBeDeleted.push_back(collider);
			}
		}
	}

	for (int i = 0; i < needToBeDeleted.size(); i++)
	{
		ecs.DeleteEntity(needToBeDeleted[i]);
	}
}
