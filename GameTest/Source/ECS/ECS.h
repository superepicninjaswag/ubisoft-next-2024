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
    ComponentPool<PhysicsBodyComponent> _physicsBodies;
    ComponentPool<TransformComponent> _transforms;
    ComponentPool<TextureComponent> _textures;
    ComponentPool<SphereColliderComponent> _sphereColliders;
public:
    void DeleteEntity(EntityDescriptor entityToDelete);
    IDManager &GetIDs();
    ComponentPool<MeshComponent>& GetMeshes();
    ComponentPool<ParticleComponent>& GetParticles();
    ComponentPool<PhysicsBodyComponent>& GetPhysicsBodies();
    ComponentPool<SphereColliderComponent>& GetSphereColliders();
    ComponentPool<TextureComponent>& GetTextures();
    ComponentPool<TransformComponent>& GetTransforms();
};

