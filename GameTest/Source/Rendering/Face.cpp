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

std::vector<Face> Face::clipAgainstPlane(Vector4 point, Vector4 normal)
{
    normal.Normalize();

    // Determine if points are inside or outside
    int numInside = 0;
    float distanceFromPlane[3];
    std::vector<Vector4> inside;
    std::vector<Vector4> outside;
    for (int i = 0; i < 3; i++)
    {
        Vector4 temp = points[i];
        distanceFromPlane[i] = normal * temp - normal * point;
        if (distanceFromPlane[i] >= 0)
        {
            numInside++;
            inside.push_back(points[i]);
        }
        else
        {
            outside.push_back(points[i]);
        }
    }

    // Create new triangles
    std::vector<Face> output;

    if (numInside == 3)
    {
        output.push_back(*this);
    }
    else if (numInside == 2)
    {
        Vector4 newPoint1 = inside[0].intersectPlane(point, normal, outside[0]);
        output.emplace_back(inside[0], inside[1], newPoint1);

        Vector4 newPoint2 = inside[1].intersectPlane(point, normal, outside[0]);
        output.emplace_back(inside[1], newPoint1, newPoint2);
    }
    else if (numInside == 1)
    {
        Vector4 newPoint1 = inside[0].intersectPlane(point, normal, outside[0]);
        Vector4 newPoint2 = inside[0].intersectPlane(point, normal, outside[1]);
        output.emplace_back(inside[0], newPoint1, newPoint2);
    }

    return output;
}
