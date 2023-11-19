#include "stdafx.h"

#include "Geometry.h"

Face::Face()
{
    
}

Face::Face(Vec3 p1, Vec3 p2, Vec3 p3)
{
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
}

bool Face::operator<(const Face &b) const
{
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