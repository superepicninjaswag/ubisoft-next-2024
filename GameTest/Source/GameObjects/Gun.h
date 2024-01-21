#pragma once

#include "../Math/Vector4.h"


class Gun
{
public:
    int m_damage;
    float m_verticalFOV;
    float m_horizontalFOV;
    float m_yawPerPixel;
    float m_pitchPerPixel;
    

    Gun(int damage, float verticalFOVDeg, float aspectRatio);
};