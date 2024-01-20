#include "stdafx.h"

#include "PhysicsBodyComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent()
{
	m_damping = 0.0f;
	m_inverseMass = 0.0f;
}

void PhysicsBodyComponent::Integrate(float deltaTime, Vector4& position)
{
	// Infinite masses shouldn't move and
	// if deltaTime is somehow 0, then nothing
	// has changed since last time.
	if (m_inverseMass > 0.0f && deltaTime > 0.0f)
	{
		position += m_velocity * deltaTime;

		Vector4 acceleration = m_gravity;
		// Update accel with forceAccumulation

		m_velocity += acceleration * deltaTime;
		m_velocity *= powf(m_damping, deltaTime);

		m_forceAccumulation.x = 0;
		m_forceAccumulation.y = 0;
		m_forceAccumulation.z = 0;
	}
}

void PhysicsBodyComponent::SetMass(float mass)
{
	if (mass != 0.0f)
	{
		m_inverseMass = 1.0f / mass;
	}
}

void PhysicsBodyComponent::SetGravity(float gravity)
{
	m_gravity.y = -gravity;
}

void PhysicsBodyComponent::SetDamping(float damping)
{
	m_damping = damping;
}
