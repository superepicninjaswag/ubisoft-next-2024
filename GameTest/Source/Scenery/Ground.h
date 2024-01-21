#pragma once

#include "../Math/Vector4.h"
#include "../ECS/ECS.h"
#include "../Rendering/MeshLibrary.h"

struct Ground
{
	Vector4 m_position;
	int m_scale = 1;
	int m_width = 1;
	int m_length = 1;

	Ground(int scale, int width, int length);
	void GenerateTiles(ECS& ecs);
	float GetMaxX();
	float GetMaxZ();
};