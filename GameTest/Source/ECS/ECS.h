#pragma once

#include "./IDManager.h"
#include "./ComponentPool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager m_ids;
    ComponentPool<DodgingAIComponent> m_dodgingAIs;
    ComponentPool<EnemyComponent> m_enemies;
    ComponentPool<HealthComponent> m_health;
    ComponentPool<LifetimeComponent> m_lifetimes;
    ComponentPool<MeshComponent> m_meshes;
    ComponentPool<ParticleComponent> m_particles;
    ComponentPool<PhysicsBodyComponent> m_physicsBodies;
    ComponentPool<PowerupComponent> m_powerups;
    ComponentPool<ProjectileComponent> m_projectiles;
    ComponentPool<SphereColliderComponent> m_sphereColliders;
    ComponentPool<SinWaveAIComponent> m_sinWaveAIs;
    ComponentPool<TransformComponent> m_transforms;
    ComponentPool<TextureComponent> m_textures;
public:
    void DeleteEntity(EntityDescriptor entityToDelete);
    IDManager &GetIDs();
    ComponentPool<DodgingAIComponent>& GetDodgingAIs();
    ComponentPool<EnemyComponent>& GetEnemies();
    ComponentPool<HealthComponent>& GetHealth();
    ComponentPool<LifetimeComponent>& GetLifetimes();
    ComponentPool<MeshComponent>& GetMeshes();
    ComponentPool<ParticleComponent>& GetParticles();
    ComponentPool<PhysicsBodyComponent>& GetPhysicsBodies();
    ComponentPool<PowerupComponent>& GetPowerups();
    ComponentPool<ProjectileComponent>& GetProjectiles();
    ComponentPool<SphereColliderComponent>& GetSphereColliders();
    ComponentPool<SinWaveAIComponent>& GetSinWaveAIs();
    ComponentPool<TextureComponent>& GetTextures();
    ComponentPool<TransformComponent>& GetTransforms();
};

