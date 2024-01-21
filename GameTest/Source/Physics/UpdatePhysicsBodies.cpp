#include "stdafx.h"

#include "UpdatePhysicsBodies.h"

void UpdatePhysicsBodies(ECS& ecs, float deltaTime)
{
	ComponentPool<PhysicsBodyComponent>& bodies = ecs.GetPhysicsBodies();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
	for (int i = 0; i < bodies.Size(); i++)
	{
		EntityDescriptor target = bodies.MirrorToEntityDescriptor(i);
		bodies.Get(target.id).Integrate(deltaTime, transforms.Get(target.id).position);
	}
}
