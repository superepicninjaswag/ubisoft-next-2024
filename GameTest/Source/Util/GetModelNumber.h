#pragma once

#include "../Components/Components.h"
#include "../ECS/Pool.h"

#include <string>

int GetModelNumber(Pool<MeshResourceComponent> &resources, std::string model);
