#include "stdafx.h"

#include "ECS.h"

IDManager &ECS::GetIDs()
{
    return _ids;
}

Pool<MeshComponent> &ECS::GetMeshes()
{
    return _meshes;
}

Pool<MeshResourceComponent> &ECS::GetMeshResources()
{
    return _meshResources;
}

Pool<TextureComponent>& ECS::GetTextures()
{
    return _textures;
}

Pool<TransformComponent> &ECS::GetTransforms()
{
    return _transforms;
}