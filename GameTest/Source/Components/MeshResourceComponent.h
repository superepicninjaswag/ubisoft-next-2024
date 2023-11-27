#pragma once

#include "../Rendering/Geometry.h"
#include "MeshResourceComponent.h"

#include <vector>
#include <string>

struct MeshResourceComponent
{
    std::vector<Face> faces;
    std::string model;
};