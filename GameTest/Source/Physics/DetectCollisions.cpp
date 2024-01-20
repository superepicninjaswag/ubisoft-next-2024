#include "stdafx.h"

#include "DetectCollisions.h"

void DetectCollisions(ECS& ecs, std::vector<Collision>& collisionQueue)
{
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

	for (int i = 0; i < spheres.Size(); i++)
	{
		for (int j = i + 1; j < spheres.Size(); j++)
		{
			EntityDescriptor colliderA = spheres.MirrorToEntityDescriptor(i);
			EntityDescriptor colliderB = spheres.MirrorToEntityDescriptor(j);
			if (transforms.Has(colliderA) && transforms.Has(colliderB))
			{
				SphereColliderComponent sphereA = spheres.Get(colliderA.id);
				SphereColliderComponent sphereB = spheres.Get(colliderB.id);

				TransformComponent transformA = transforms.Get(colliderA.id);
				TransformComponent transformB = transforms.Get(colliderB.id);

				// Section 4.3.1 in "Real-Time Collision Detection" by Christer Ericson
				Vector4 d = transformA.position - transformB.position;
				float dist2 = d * d;
				float radiusSum = sphereA.m_radius + sphereB.m_radius;

				if (dist2 <= radiusSum * radiusSum)
				{
					collisionQueue.emplace_back(colliderA, colliderB);
				}
			}
		}
	}
}
