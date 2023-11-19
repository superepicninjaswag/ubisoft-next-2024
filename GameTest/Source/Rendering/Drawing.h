#pragma once

#include "../ECS/Pool.h"
#include "../Components/Components.h"
#include "./Geometry.h"


extern Matrix4x4 g_projectionMatrix;

void drawTriangle(const Face& triangle, const Colour c);
void drawFilledTriangle(const Face& triangle, const Colour c);