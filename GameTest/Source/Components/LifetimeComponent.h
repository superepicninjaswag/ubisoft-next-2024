#pragma once


struct LifetimeComponent
{
	int m_remainingLifetimeFrames = 0;

	LifetimeComponent(int remainingLifetimeFrames);
};