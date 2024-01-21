#include "stdafx.h"

#include "Systems.h"

void AnimateSinWaveAI(ECS& ecs, EntityDescriptor player)
{
	ComponentPool<SinWaveAIComponent>& sinWaveAIs = ecs.GetSinWaveAIs();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
	Vector4 playerLocation = transforms.Get(player.id).position;

	for (int i = 0; i < sinWaveAIs.Size(); i++)
	{
		EntityDescriptor ai = sinWaveAIs.MirrorToEntityDescriptor(i);
		Vector4 aiLocation = transforms.Get(ai.id).position;

		Vector4 direction = playerLocation - aiLocation;
		direction.Normalize();

		direction.y += 1.25f * sinf(sinWaveAIs.Get(ai.id).theta);

		transforms.Get(ai.id).position += direction * sinWaveAIs.Get(ai.id).SPEED;
		sinWaveAIs.Get(ai.id).theta += 0.1f;
	}
}