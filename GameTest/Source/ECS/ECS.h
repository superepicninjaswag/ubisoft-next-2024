#pragma once

#include "./IDManager.h"
#include "./Pool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager _ids;
    Pool<MeshComponent> _meshes;
    Pool<MeshResourceComponent> _meshResources;
    Pool<TransformComponent> _transforms;
public:
    IDManager &GetIDs();
    Pool<MeshComponent> &GetMeshes();
    Pool<MeshResourceComponent> &GetMeshResources();
    Pool<TransformComponent> &GetTransforms();
};

