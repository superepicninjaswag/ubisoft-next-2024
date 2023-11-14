#pragma once

#include "../Util/GameMath.h"

struct Colour
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct Face
{
public:
    Vec3 points[3];
    Colour colour;

    Face();
    Face(Vec3 p1, Vec3 p2, Vec3 p3);
    bool operator<(const Face &b) const;
};