#pragma once

#include <string>

#include "../Rendering/Face.h"

struct MeshComponent
{
    std::string assetName;

    MeshComponent(std::string assignedAssetName);
};
