#include "stdafx.h"

#include "ParticleComponent.h"

ParticleComponent::ParticleComponent(float x, float y, float z, int lifetime)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;

	remainingLifetimeFrames = lifetime;
}
