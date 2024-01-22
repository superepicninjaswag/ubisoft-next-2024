#include "stdafx.h"

#include "EntityBuilder.h"

EntityBuilder::EntityBuilder(ECS& ecs) : m_ecs(ecs)
{
	Reset();
}

void EntityBuilder::Reset()
{
	m_entity = m_ecs.GetIDs().CreateId();
}

void EntityBuilder::SetEnemy()
{
	if (!m_ecs.GetEnemies().Has(m_entity))
	{
		m_ecs.GetEnemies().Add(m_entity);
	}
}

void EntityBuilder::SetHealth(int health)
{
	if (!m_ecs.GetHealth().Has(m_entity))
	{
		m_ecs.GetHealth().Add(m_entity, health);
	}
}

void EntityBuilder::SetLifetime(int frameLifetime)
{
	if (!m_ecs.GetLifetimes().Has(m_entity))
	{
		m_ecs.GetLifetimes().Add(m_entity, frameLifetime);
	}
}

void EntityBuilder::SetMesh(int meshCode)
{
	if (!m_ecs.GetMeshes().Has(m_entity))
	{
		m_ecs.GetMeshes().Add(m_entity, meshCode);
	}
}

void EntityBuilder::SetPhysicsBody()
{
	if (!m_ecs.GetPhysicsBodies().Has(m_entity))
	{
		m_ecs.GetPhysicsBodies().Add(m_entity);
	}
}

void EntityBuilder::SetProjectile(int damage)
{
	if (!m_ecs.GetProjectiles().Has(m_entity))
	{
		m_ecs.GetProjectiles().Add(m_entity, damage);
	}
}

void EntityBuilder::SetAI(int aiCode)
{
	switch (aiCode)
	{
		case AICode::SINWAVE:
			if (!m_ecs.GetSinWaveAIs().Has(m_entity))
			{
				m_ecs.GetSinWaveAIs().Add(m_entity);
			}
			break;
		case AICode::DODGING:
			if (!m_ecs.GetDodgingAIs().Has(m_entity))
			{
				m_ecs.GetDodgingAIs().Add(m_entity);
			}
			break;

		default:
			// Don't add
			break;
	}

}

void EntityBuilder::SetCollider()
{
	// TODO: This is duct taped spaghetti garbage. It assumes the scale is uniform between the axes. Please fix.
	if (!m_ecs.GetSphereColliders().Has(m_entity) && m_ecs.GetTransforms().Has(m_entity) && m_ecs.GetMeshes().Has(m_entity))
	{
		switch (m_ecs.GetMeshes().Get(m_entity.id).meshAssetCode)
		{
			case MeshLibrary::CONE :
				m_ecs.GetSphereColliders().Add(m_entity, 1.414f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::CUBE :
				m_ecs.GetSphereColliders().Add(m_entity, 1.732f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::CYLINDER :
				m_ecs.GetSphereColliders().Add(m_entity, 1.414f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::ICOSPHERE :
				m_ecs.GetSphereColliders().Add(m_entity, 1.0f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::MONKEY :
				m_ecs.GetSphereColliders().Add(m_entity, 0.986f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::PLANE :
				m_ecs.GetSphereColliders().Add(m_entity, 1.414f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::TORUS :
				m_ecs.GetSphereColliders().Add(m_entity, 1.25f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
			case MeshLibrary::UVSPHERE :
				m_ecs.GetSphereColliders().Add(m_entity, 1.0f * m_ecs.GetTransforms().Get(m_entity.id).scale.x);
				break;
		}
	}
}

void EntityBuilder::SetTexture(Colour outline, Colour fill)
{
	if (!m_ecs.GetTextures().Has(m_entity))
	{
		m_ecs.GetTextures().Add(m_entity, outline, fill);
	}
}

void EntityBuilder::SetTransform(float x, float y, float z)
{
	if (!m_ecs.GetTransforms().Has(m_entity))
	{
		m_ecs.GetTransforms().Add(m_entity, x, y, z);
	}
}

void EntityBuilder::SetScale(float scale)
{
	if (m_ecs.GetTransforms().Has(m_entity))
	{
		m_ecs.GetTransforms().Get(m_entity.id).scale *= scale;
	}
}

void EntityBuilder::SetPowerup()
{
	if (!m_ecs.GetPowerups().Has(m_entity))
	{
		m_ecs.GetPowerups().Add(m_entity);
	}
}
