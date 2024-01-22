#include "stdafx.h"

#include "Systems.h"

void PowerupSpawner(ECS& ecs, Ground ground)
{
	if (ecs.GetPowerups().Size() < 2)
	{
		EntityBuilder builder(ecs);
		Colour blue(0.0f, 0.0f, 1.0f);
		Colour grey(0.5f, 0.5f, 0.5f);

		float randomX = rand() % ((int)ground.m_scale * 2 * (int)ground.m_width) - ((int)ground.m_scale * 2 * (int)ground.m_width) / 2.0f;
		float notRandomY = 16.0f;
		float randomZ = rand() % ((int)ground.m_scale * 2 * (int)ground.m_length) - ((int)ground.m_scale * 2 * (int)ground.m_length) / 2.0f;
		builder.SetTransform(randomX, notRandomY, randomZ);
		builder.SetScale(5.0f);
		builder.SetMesh(MeshLibrary::MONKEY);
		builder.SetTexture(grey, blue);
		builder.SetCollider();
		builder.SetPowerup();
	}
}
