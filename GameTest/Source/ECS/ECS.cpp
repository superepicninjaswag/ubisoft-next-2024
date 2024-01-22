#include "stdafx.h"

#include "ECS.h"

void ECS::DeleteEntity(EntityDescriptor entityToDelete)
{
    m_enemies.Delete(entityToDelete);
    m_health.Delete(entityToDelete);
    m_lifetimes.Delete(entityToDelete);
    m_meshes.Delete(entityToDelete);
    m_particles.Delete(entityToDelete);
    m_physicsBodies.Delete(entityToDelete);
    m_powerups.Delete(entityToDelete);
    m_projectiles.Delete(entityToDelete);
    m_sphereColliders.Delete(entityToDelete);
    m_sinWaveAIs.Delete(entityToDelete);
    m_textures.Delete(entityToDelete);
    m_transforms.Delete(entityToDelete);
    m_ids.DeleteId(entityToDelete);
}

IDManager &ECS::GetIDs()
{
    return m_ids;
}

ComponentPool<EnemyComponent>& ECS::GetEnemies()
{
    return m_enemies;
}

ComponentPool<HealthComponent>& ECS::GetHealth()
{
    return m_health;
}

ComponentPool<LifetimeComponent>& ECS::GetLifetimes()
{
    return m_lifetimes;
}

ComponentPool<MeshComponent>& ECS::GetMeshes()
{
    return m_meshes;
}

ComponentPool<ParticleComponent>& ECS::GetParticles()
{
    return m_particles;
}

ComponentPool<PhysicsBodyComponent>& ECS::GetPhysicsBodies()
{
    return m_physicsBodies;
}

ComponentPool<PowerupComponent>& ECS::GetPowerups()
{
    return m_powerups;
}

ComponentPool<ProjectileComponent>& ECS::GetProjectiles()
{
    return m_projectiles;
}

ComponentPool<SphereColliderComponent>& ECS::GetSphereColliders()
{
    return m_sphereColliders;
}

ComponentPool<SinWaveAIComponent>& ECS::GetSinWaveAIs()
{
    return m_sinWaveAIs;
}

ComponentPool<TextureComponent>& ECS::GetTextures()
{
    return m_textures;
}

ComponentPool<TransformComponent>& ECS::GetTransforms()
{
    return m_transforms;
}