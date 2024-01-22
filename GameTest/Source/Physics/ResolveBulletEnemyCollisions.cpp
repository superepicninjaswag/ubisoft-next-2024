#include "stdafx.h"

#include "ResolveBulletEnemyCollisions.h"

void ResolveBulletEnemyCollisions(ECS& ecs)
{
	ComponentPool<EnemyComponent>& enemies = ecs.GetEnemies();
	ComponentPool<ProjectileComponent>& bullets = ecs.GetProjectiles();
	ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
	ComponentPool<HealthComponent>& health = ecs.GetHealth();

	std::vector<EntityDescriptor> bulletsToDelete;
	for (int i = 0; i < enemies.Size(); i++)
	{
		for (int j = 0; j < bullets.Size(); j++)
		{
			EntityDescriptor enemy = enemies.MirrorToEntityDescriptor(i);
			EntityDescriptor bullet = bullets.MirrorToEntityDescriptor(j);

			SphereColliderComponent colliderEnemy = spheres.Get(enemy.id);
			SphereColliderComponent colliderBullet = spheres.Get(bullet.id);

			TransformComponent transformEnemy = transforms.Get(enemy.id);
			TransformComponent transformBullet = transforms.Get(bullet.id);

			// From 4.3.1 in "Real-Time Collision Detection" by Christer Ericson
			Vector4 d = transformEnemy.position - transformBullet.position;
			float dist2 = d * d;
			float radiusSum = colliderEnemy.m_radius + colliderBullet.m_radius;
			if (dist2 <= radiusSum * radiusSum)
			{
				bulletsToDelete.push_back(bullet);
				health.Get(enemy.id).m_health -= bullets.Get(bullet.id).m_damage;
			}
		}
	}

	for (int i = 0; i < bulletsToDelete.size(); i++)
	{
		ecs.DeleteEntity(bulletsToDelete[i]);
	}
}
