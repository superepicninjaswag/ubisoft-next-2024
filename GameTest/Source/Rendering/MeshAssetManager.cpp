#include "stdafx.h"

#include "MeshAssetManager.h"

MeshAssetManager::MeshAssetManager() {};

void MeshAssetManager::LoadMeshAsset(std::string assetName)
{
    std::string pathPrefix = "./Assets/Meshes/";
    std::string pathSuffix = ".obj";

    std::ifstream file(pathPrefix + assetName + pathSuffix);

    if (!file.is_open())
    {
        // TODO: Quit with error message. For now I will let the program crash and burn
        return;
    }

    /* I'm going to assume the given .obj files are nice. ie they define vertices before faces */
    std::vector<Vector4> vertices;
    std::vector<Face> faces;
    char leadingCharacter;
    float x, y, z;
    int p1, p2, p3;
    while (file >> leadingCharacter)
    {
        if (leadingCharacter == 'v')
        {
            file >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (leadingCharacter == 'f')
        {
            file >> p1 >> p2 >> p3;
            faces.emplace_back(vertices[p1 - 1], vertices[p2 - 1], vertices[p3 - 1]);
        }
    }

    file.close();

    assets[assetName] = faces;
}

