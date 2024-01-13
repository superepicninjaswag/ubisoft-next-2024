#include "stdafx.h"

#include "Face.h"


Face::Face() {}

Face::Face(Vector4 p1, Vector4 p2, Vector4 p3)
{
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
}

bool Face::operator<(const Face &b) const
{
    // Average of z values
    float zSumFaceA = 0.0f;
    float zSumFaceB = 0.0f;
    for(int i = 0; i < 3; i++)
    {
        zSumFaceA += points[i].z;
        zSumFaceB += b.points[i].z;
    }
    zSumFaceA /= 3;
    zSumFaceB /= 3;
    return zSumFaceA > zSumFaceB;
}

bool Face::isWithinNDCCube()
{
    // Right boundary
    if (points[0].x > 1.0 && points[1].x > 1.0 && points[2].x > 1.0)
    {
        return false;
    }
    // Top boundary
    else if (points[0].y > 1.0 && points[1].y > 1.0 && points[2].y > 1.0)
    {
        return false;
    }
    // Far boundary
    else if (points[0].z > 1.0 && points[1].z > 1.0 && points[2].z > 1.0)
    {
        return false;
    }
    // Left boundary
    else if (points[0].x < -1.0 && points[1].x < -1.0 && points[2].x < -1.0)
    {
        return false;
    }
    // Bottom boundary
    else if (points[0].y < -1.0 && points[1].y < -1.0 && points[2].y < -1.0)
    {
        return false;
    }

    return true;
}
