#pragma once

#include "../Math/Vec3.h"

struct Face
{
public:
    Vec3 points[3];
    int entityId = -1;

    Face();
    Face(Vec3 p1, Vec3 p2, Vec3 p3);
    bool operator<(const Face &b) const;
    std::vector<Face> clipAgainstPlane(Vec3 point, Vec3 normal);
};