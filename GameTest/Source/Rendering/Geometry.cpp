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

std::vector<Face> Face::clipAgainstPlane(Vec3 point, Vec3 normal)
{
    normal.Normalize();

    // Determine if points are inside or outside
    int numInside = 0;
    float distanceFromPlane[3];
    std::vector<Vec3> inside;
    std::vector<Vec3> outside;
    for (int i = 0; i < 3; i++)
    {
        Vec3 temp = points[i];
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
        Vec3 newPoint1 = inside[0].intersectPlane(point, normal, outside[0]);
        output.emplace_back(inside[0], inside[1], newPoint1);
        output[output.size() - 1].colour.r = 1.0f;
        output[output.size() - 1].colour.g = .0f;
        output[output.size() - 1].colour.b = .0f;

        Vec3 newPoint2 = inside[1].intersectPlane(point, normal, outside[0]);
        output.emplace_back(inside[1], newPoint1, newPoint2);
        output[output.size() - 1].colour.r = 1.0f;
        output[output.size() - 1].colour.g = .0f;
        output[output.size() - 1].colour.b = .0f;

        //std::cout << newPoint1.x << " | " << newPoint1.y << " | " << newPoint2.x << " | " << newPoint2.y << "\n";
    }
    else if (numInside == 1)
    {
        Vec3 newPoint1 = inside[0].intersectPlane(point, normal, outside[0]);
        Vec3 newPoint2 = inside[0].intersectPlane(point, normal, outside[1]);
        output.emplace_back(inside[0], newPoint1, newPoint2);
        output[output.size() - 1].colour.r = .0f;
        output[output.size() - 1].colour.g = 1.0f;
        output[output.size() - 1].colour.b = .0f;
    }

    return output;
}
