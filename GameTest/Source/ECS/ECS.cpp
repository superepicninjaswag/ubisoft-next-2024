#include "stdafx.h"

#include "ECS.h"

IDManager &ECS::GetIDs()
{
    return _ids;
}

ComponentPool<MeshComponent> &ECS::GetMeshes()
{
    return _meshes;
}

ComponentPool<ParticleComponent> &ECS::GetParticles()
{
    return _particles;
}

ComponentPool<TextureComponent> &ECS::GetTextures()
{
    return _textures;
}

ComponentPool<TransformComponent> &ECS::GetTransforms()
{
    return _transforms;
}