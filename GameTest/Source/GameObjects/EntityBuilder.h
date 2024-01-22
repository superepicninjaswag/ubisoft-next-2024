#pragma once

#include "../ECS/ECS.h"
#include "../ECS/EntityDescriptor.h"
#include "../Rendering/MeshLibrary.h"


class EntityBuilder
{
private:
	ECS& m_ecs;
	EntityDescriptor m_entity;

public:
	enum AICode
	{
		SINWAVE
	};

	EntityBuilder(ECS& ecs);
	void Reset();
	void SetEnemy();
	void SetHealth(int health);
	void SetLifetime(int frameLifetime);
	void SetMesh(int meshCode);
	void SetPhysicsBody();
	void SetProjectile(int damage);
	void SetAI(int aiCode);
	void SetCollider();
	void SetTexture(Colour outline, Colour fill);
	void SetTransform(float x, float y, float z);
};