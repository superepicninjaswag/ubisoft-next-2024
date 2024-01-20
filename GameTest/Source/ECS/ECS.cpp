#include "stdafx.h"

#include "ECS.h"

void ECS::DeleteEntity(EntityDescriptor entityToDelete)
{
    _meshes.Delete(entityToDelete);
    _particles.Delete(entityToDelete);
    _physicsBodies.Delete(entityToDelete);
    _textures.Delete(entityToDelete);
    _transforms.Delete(entityToDelete);
    _sphereColliders.Delete(entityToDelete);
    _ids.DeleteId(entityToDelete);
}

IDManager &ECS::GetIDs()
{
    return _ids;
}

ComponentPool<MeshComponent>& ECS::GetMeshes()
{
    return _meshes;
}

ComponentPool<ParticleComponent>& ECS::GetParticles()
{
    return _particles;
}

ComponentPool<PhysicsBodyComponent>& ECS::GetPhysicsBodies()
{
    return _physicsBodies;
}

ComponentPool<SphereColliderComponent>& ECS::GetSphereColliders()
{
    return _sphereColliders;
}

ComponentPool<TextureComponent>& ECS::GetTextures()
{
    return _textures;
}

ComponentPool<TransformComponent>& ECS::GetTransforms()
{
    return _transforms;
}