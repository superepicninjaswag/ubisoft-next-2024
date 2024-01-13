#pragma once

#include "../Math/Vector4.h"

struct Face
{
public:
    Vector4 points[3];
    int entityId = -1;

    Face();
    Face(Vector4 p1, Vector4 p2, Vector4 p3);
    bool operator<(const Face &b) const;
    bool isWithinNDCCube();
};