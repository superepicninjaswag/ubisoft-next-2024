#pragma once

#include "../ECS/ComponentPool.h"
#include "../Components/Components.h"
#include "./Face.h"
#include "../Math/Matrix4.h"


extern Matrix4 g_cameraSpaceToClipSpaceTransform;

void drawTriangle(const Face& triangle, const Colour c);
void drawFilledTriangle(const Face& triangle, const Colour c);