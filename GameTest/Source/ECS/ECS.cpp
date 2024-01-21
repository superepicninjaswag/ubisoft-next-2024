#include "stdafx.h"

#include "ECS.h"

void ECS::DeleteEntity(EntityDescriptor entityToDelete)
{
    m_meshes.Delete(entityToDelete);
    m_particles.Delete(entityToDelete);
    m_physicsBodies.Delete(entityToDelete);
    m_textures.Delete(entityToDelete);
    m_transforms.Delete(entityToDelete);
    m_sphereColliders.Delete(entityToDelete);
    m_ids.DeleteId(entityToDelete);
}

IDManager &ECS::GetIDs()
{
    return m_ids;
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

ComponentPool<SphereColliderComponent>& ECS::GetSphereColliders()
{
    return m_sphereColliders;
}

ComponentPool<TextureComponent>& ECS::GetTextures()
{
    return m_textures;
}

ComponentPool<TransformComponent>& ECS::GetTransforms()
{
    return m_transforms;
}