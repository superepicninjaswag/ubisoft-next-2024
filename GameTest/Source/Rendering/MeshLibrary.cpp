#include "stdafx.h"

#include "MeshLibrary.h"

MeshLibrary::MeshLibrary() : meshData(8)
{
    LoadMeshAsset("./Assets/Meshes/cone.obj", MeshCode::CONE);
    LoadMeshAsset("./Assets/Meshes/cube.obj", MeshCode::CUBE);
    LoadMeshAsset("./Assets/Meshes/cylinder.obj", MeshCode::CYLINDER);
    LoadMeshAsset("./Assets/Meshes/icosphere.obj", MeshCode::ICOSPHERE);
    LoadMeshAsset("./Assets/Meshes/monkey.obj", MeshCode::MONKEY);
    LoadMeshAsset("./Assets/Meshes/plane.obj", MeshCode::PLANE);
    LoadMeshAsset("./Assets/Meshes/torus.obj", MeshCode::TORUS);
    LoadMeshAsset("./Assets/Meshes/uvsphere.obj", MeshCode::UVSPHERE);
}
std::vector<Face>& MeshLibrary::operator[](int assetCode)
{
    return meshData[assetCode];
}
;

void MeshLibrary::LoadMeshAsset(std::string assetPath, int meshCode)
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
            meshData[meshCode].emplace_back(vertices[p1 - 1], vertices[p2 - 1], vertices[p3 - 1]);
        }
    }

    file.close();
}

