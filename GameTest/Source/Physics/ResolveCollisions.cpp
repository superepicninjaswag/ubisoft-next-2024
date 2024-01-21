#include "stdafx.h"

#include "ResolveCollisions.h"

void ResolveCollisions(ECS& ecs)
{
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

	for (int i = 0; i < spheres.Size(); i++)
	{
		EntityDescriptor collider = spheres.MirrorToEntityDescriptor(i);
		SphereColliderComponent sphere = spheres.Get(collider.id);
		TransformComponent transform = transforms.Get(collider.id);

		// Resolve Collision
	}
}
