#pragma once

#include "../ECS/ECS.h"
#include "../ECS/ComponentPool.h"
#include "../Rendering/Face.h"
#include "../Math/Vector4.h"

class MeshAssetManager
{
public:
    std::unordered_map<int, std::vector<Face>> assets;
    enum MeshAssetCodes
    {
        CONE,
        CUBE,
        CYLINDER,
        ICOSPHERE,
        MONKEY,
        PLANE,
        TORUS,
        UVSPHERE
    };

    MeshAssetManager();
    void LoadMeshAsset(std::string assetName, int assetCode);
};