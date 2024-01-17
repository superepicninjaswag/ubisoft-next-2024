#pragma once

#include "../ECS/ECS.h"
#include "../ECS/ComponentPool.h"
#include "../Rendering/Face.h"
#include "../Math/Vector4.h"

class MeshAssetManager
{
public:
    std::unordered_map<std::string, std::vector<Face>> assets;

    MeshAssetManager();
    void LoadMeshAsset(std::string assetName);
};