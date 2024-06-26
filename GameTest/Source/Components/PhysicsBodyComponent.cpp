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

		Vector4 m_acceleration = m_gravity;	// We want some gravity by default
		m_acceleration += m_forceAccumulation * m_inverseMass;

		m_velocity += m_acceleration * deltaTime;
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

void PhysicsBodyComponent::AddForce(const Vector4 force)
{
	m_forceAccumulation += force;
}
