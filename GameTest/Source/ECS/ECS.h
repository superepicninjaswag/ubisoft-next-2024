#pragma once

#include "./IDManager.h"
#include "./Pool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager _ids;
    Pool<MeshComponent> _meshes;
    Pool<TransformComponent> _transforms;
    Pool<TextureComponent> _textures;
public:
    IDManager &GetIDs();
    Pool<MeshComponent> &GetMeshes();
    Pool<TextureComponent>& GetTextures();
    Pool<TransformComponent> &GetTransforms();
};

