#pragma once

#include "../Math/Vector4.h"


struct PhysicsBodyComponent
{
	Vector4 m_position;
	Vector4 m_velocity;
	Vector4 m_acceleration;

	PhysicsBodyComponent();
};