#include "stdafx.h"

#include "Systems.h"

void EnemySpawner(ECS& ecs, float deltaTime, Ground ground)
{
	static float spawnTimer = 0.0f;

	if (spawnTimer > 0.0f)
	{
		spawnTimer -= deltaTime;
	}
	else
	{
		spawnTimer = 2.0f;

		EntityBuilder builder(ecs);
		Colour red(1.0f, 0.0f, 0.0f);
		Colour orange(1.0f, 0.647f, 0.0f);

		float randomX = rand() % ((int) ground.m_scale * 2 * (int) ground.m_width) - ((int)ground.m_scale * 2 * (int)ground.m_width)/2.0f;
		float randomY = (rand() % 100) + 100.0f;
		float notRandomZ = (ground.m_scale * 2 * (int)ground.m_length)/2.0f + 50.0f;
		builder.SetTransform(randomX, randomY, notRandomZ);
		builder.SetMesh(MeshLibrary::CUBE);
		builder.SetTexture(orange, red);
		builder.SetCollider();
		builder.SetAI(EntityBuilder::SINWAVE);
		builder.SetEnemy();
		builder.SetHealth(1);
	}
}
