#include "stdafx.h"

#include "Systems.h"

void AnimateDodgingAI(ECS& ecs, EntityDescriptor player, Gun gun)
{
	ComponentPool<DodgingAIComponent>& dodgingAIs = ecs.GetDodgingAIs();
	ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

	for (int i = 0; i < dodgingAIs.Size(); i++)
	{
		EntityDescriptor ai = dodgingAIs.MirrorToEntityDescriptor(i);

		Vector4 aiLocation = transforms.Get(ai.id).position;
		Vector4 playerLocation = transforms.Get(player.id).position;

		Vector4 direction = playerLocation - aiLocation;
		direction.Normalize();

		Vector4 playerGaze = gun.m_launchDirection * -1.0f;
		playerGaze.Normalize();

		// Update state
		if (direction * playerGaze > 0.9659f)
		{
			dodgingAIs.Get(ai.id).currentState = DodgingAIComponent::DODGE;
		}
		else
		{
			dodgingAIs.Get(ai.id).currentState = DodgingAIComponent::ATTACK;
		}

		// Animate
		if (dodgingAIs.Get(ai.id).currentState == DodgingAIComponent::ATTACK)
		{
			transforms.Get(ai.id).position += direction;
		}
		else if (dodgingAIs.Get(ai.id).currentState == DodgingAIComponent::DODGE)
		{
			// Do nothing for now. Sort of like Boo from mario. It's supposed to dodge but I ran out of time
			// I would have computed a random vector perpendicular to "direction" and traveled along that until the state changed
		}
	}
}