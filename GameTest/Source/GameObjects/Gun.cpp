#include "stdafx.h"

#include "Gun.h"

Gun::Gun(int damage, float verticalFOVDeg, float aspectRatio) : m_damage(damage)
{
	m_verticalFOV = verticalFOVDeg * (acosf(-1.0f) / 180.0f);
	m_horizontalFOV = 2.0f * atanf(tanf(m_verticalFOV / 2.0f) * aspectRatio);
	m_yawPerPixel = m_horizontalFOV / APP_VIRTUAL_WIDTH;
	m_pitchPerPixel = m_verticalFOV / APP_VIRTUAL_HEIGHT;
}

