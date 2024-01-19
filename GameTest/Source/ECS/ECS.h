#pragma once

#include "./IDManager.h"
#include "./ComponentPool.h"
#include "../Components/Components.h"


class ECS
{
private:
    IDManager _ids;
    ComponentPool<MeshComponent> _meshes;
    ComponentPool<ParticleComponent> _particles;
    ComponentPool<TransformComponent> _transforms;
    ComponentPool<TextureComponent> _textures;
public:
    void DeleteEntity(EntityDescriptor entityToDelete);
    IDManager &GetIDs();
    ComponentPool<MeshComponent> &GetMeshes();
    ComponentPool<ParticleComponent> &GetParticles();
    ComponentPool<TextureComponent> &GetTextures();
    ComponentPool<TransformComponent> &GetTransforms();
};

