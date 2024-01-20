#include "stdafx.h"

#include "ResolveCollisions.h"

void ResolveCollisions(ECS& ecs, std::vector<Contact>& contactQueue)
{
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

	for (int i = 0; i < contactQueue.size(); i++)
	{
		EntityDescriptor colliderA = contactQueue[i].m_entityA;
		EntityDescriptor colliderB = contactQueue[i].m_entityB;

		SphereColliderComponent sphereA = spheres.Get(colliderA.id);
		SphereColliderComponent sphereB = spheres.Get(colliderB.id);

		TransformComponent transformA = transforms.Get(colliderA.id);
		TransformComponent transformB = transforms.Get(colliderB.id);

		// Resolve Collision
	}

	contactQueue.clear();
}
