#pragma once

#include "../ECS/ECS.h"
#include "../ECS/ComponentPool.h"
#include "../Rendering/Face.h"
#include "../Math/Vector4.h"


enum MeshCode
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

class MeshLibrary
{
private:
    std::vector<std::vector<Face>> meshData;
    void LoadMeshAsset(std::string assetPath, int meshCode);
public:
    MeshLibrary();
    std::vector<Face>& operator[](int assetCode);
};