#include "stdafx.h"

#include "MeshResourceObjLoader.h"

void MeshResourceObjLoader(ECS &ecs)
{
    std::string pathPrefix = "./Assets/Meshes/";
    std::string pathSuffix = ".obj";
    std::vector<std::string> models = {
        "cone",
        "cube",
        "cylinder",
        "icosphere",
        "monkey",
        "plane",
        "torus",
        "uvsphere"
    };

    for (auto m : models)
    {
        std::ifstream file(pathPrefix + m + pathSuffix);
        
        if (!file.is_open())
        {
            // TODO: Quit with error message
            // For now I don't care and will let the program crash and burn
            return;
        }

        /* I'm going to assume the given .obj files are nice. ie they define vertices before faces */
        std::vector<Vector4> vertices;
        std::vector<Face> faces;
        char leadingCharacter;
        float x, y, z;
        int p1, p2, p3;
        while(file >> leadingCharacter)
        {
            if (leadingCharacter == 'v')
            {
                file >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }
            else if (leadingCharacter == 'f')
            {
                file >> p1 >> p2 >> p3;
                faces.emplace_back(vertices[p1-1], vertices[p2-1], vertices[p3-1]);
            }
        }

        file.close();

        MeshResourceComponent resource;
        resource.faces = faces;
        resource.model = m;
        int meshId = ecs.GetIDs().CreateId();
        ecs.GetMeshResources().Add(meshId, resource);
    }
}