#pragma once

#include "../Math/Vector4.h"


struct PhysicsBodyComponent
{
public:
	PhysicsBodyComponent();
	void Integrate(float deltaTime, Vector4& position);
	void SetMass(float mass);
	void SetGravity(float gravity);
	void SetDamping(float damping);

private:
	Vector4 m_velocity;
	Vector4 m_forceAccumulation;
	Vector4 m_gravity;

	float	m_damping;
	float	m_inverseMass;
};