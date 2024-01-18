#pragma once

#include "../Rendering/Face.h"

struct MeshComponent
{
    std::vector<Face> &meshData;

    MeshComponent(std::vector<Face>& assignedMeshData);
};
