#pragma once

#include "./IDManager.h"
#include "./ComponentPool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager _ids;
    ComponentPool<MeshComponent> _meshes;
    ComponentPool<TransformComponent> _transforms;
    ComponentPool<TextureComponent> _textures;
public:
    IDManager &GetIDs();
    ComponentPool<MeshComponent> &GetMeshes();
    ComponentPool<TextureComponent>& GetTextures();
    ComponentPool<TransformComponent> &GetTransforms();
};

