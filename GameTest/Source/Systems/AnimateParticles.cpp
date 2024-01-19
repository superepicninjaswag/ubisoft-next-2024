#include "stdafx.h"

#include "AnimateParticles.h"


void AnimateParticles(ECS& ecs)
{
	ComponentPool<ParticleComponent>& particles = ecs.GetParticles();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
	for (int i = 0; i < particles.Size(); i++)
	{
		EntityDescriptor targetEntity = particles.MirrorToEntityDescriptor(i);
		if (transforms.Has(targetEntity))
		{
			if (particles.Get(targetEntity.id).remainingLifetimeFrames == 0)
			{
				ecs.DeleteEntity(targetEntity);
			}
			else
			{
				transforms.Get(targetEntity.id).position = transforms.Get(targetEntity.id).position + particles.Get(targetEntity.id).velocity;
				particles.Get(targetEntity.id).remainingLifetimeFrames--;
			}
		}
	}
}
