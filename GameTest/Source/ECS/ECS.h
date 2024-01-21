#pragma once

#include "./IDManager.h"
#include "./ComponentPool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager m_ids;
    ComponentPool<LifetimeComponent> m_lifetimes;
    ComponentPool<MeshComponent> m_meshes;
    ComponentPool<ParticleComponent> m_particles;
    ComponentPool<PhysicsBodyComponent> m_physicsBodies;
    ComponentPool<ProjectileComponent> m_projectiles;
    ComponentPool<TransformComponent> m_transforms;
    ComponentPool<TextureComponent> m_textures;
    ComponentPool<SphereColliderComponent> m_sphereColliders;
public:
    void DeleteEntity(EntityDescriptor entityToDelete);
    IDManager &GetIDs();
    ComponentPool<LifetimeComponent>& GetLifetimes();
    ComponentPool<MeshComponent>& GetMeshes();
    ComponentPool<ParticleComponent>& GetParticles();
    ComponentPool<PhysicsBodyComponent>& GetPhysicsBodies();
    ComponentPool<ProjectileComponent>& GetProjectiles();
    ComponentPool<SphereColliderComponent>& GetSphereColliders();
    ComponentPool<TextureComponent>& GetTextures();
    ComponentPool<TransformComponent>& GetTransforms();
};

