#include "stdafx.h"

#include "MeshLibrary.h"

MeshLibrary::MeshLibrary()
{
    LoadMeshAsset("./Assets/Meshes/cone.obj", &cone);
    LoadMeshAsset("./Assets/Meshes/cube.obj", &cube);
    LoadMeshAsset("./Assets/Meshes/cylinder.obj", &cylinder);
    LoadMeshAsset("./Assets/Meshes/icosphere.obj", &icosphere);
    LoadMeshAsset("./Assets/Meshes/monkey.obj", &monkey);
    LoadMeshAsset("./Assets/Meshes/plane.obj", &plane);
    LoadMeshAsset("./Assets/Meshes/torus.obj", &torus);
    LoadMeshAsset("./Assets/Meshes/uvsphere.obj", &uvsphere);
};

void MeshLibrary::LoadMeshAsset(std::string assetPath, std::vector<Face> *destination)
{
    std::ifstream file(assetPath);

    if (!file.is_open())
    {
        // TODO: Quit with error message. For now I will let the program crash and burn
        return;
    }

    /* I'm going to assume the given .obj files are nice. ie they define vertices before faces */
    std::vector<Vector4> vertices;
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
            destination->emplace_back(vertices[p1 - 1], vertices[p2 - 1], vertices[p3 - 1]);
        }
    }

    file.close();
}

