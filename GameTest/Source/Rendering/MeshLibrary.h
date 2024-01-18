#pragma once

#include "../ECS/ECS.h"
#include "../ECS/ComponentPool.h"
#include "../Rendering/Face.h"
#include "../Math/Vector4.h"

class MeshLibrary
{
private:
    void LoadMeshAsset(std::string assetPath, std::vector<Face> *destination);

public:
    std::vector<Face> cone;
    std::vector<Face> cube;
    std::vector<Face> cylinder;
    std::vector<Face> icosphere;
    std::vector<Face> monkey;
    std::vector<Face> plane;
    std::vector<Face> torus;
    std::vector<Face> uvsphere;


    MeshLibrary();
};