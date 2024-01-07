#pragma once

#include "../Rendering/Face.h"

struct MeshResourceComponent
{
    std::vector<Face> faces;
    std::string model;
};