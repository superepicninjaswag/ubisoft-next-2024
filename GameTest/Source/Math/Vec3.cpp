#include "stdafx.h"

#include "Vec3.h"

Vec3::Vec3()
{

}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1.0f;
}

Vec3 Vec3::operator+(const Vec3 &i)
{
    Vec3 o;
    o.x = x + i.x;
    o.y = y + i.y;
    o.z = z + i.z;
    return o;
}

Vec3 Vec3::operator-(const Vec3 &i)
{
    Vec3 o;
    o.x = x - i.x;
    o.y = y - i.y;
    o.z = z - i.z;
    return o;
}

float Vec3::operator*(const Vec3 &i)
{
    return x*i.x + y*i.y + z*i.z;
}

Vec3 Vec3::operator*(const float &i)
{
    Vec3 o;
    o.x = x * i;
    o.y = y * i;
    o.z = z * i;
    return o;
}

void Vec3::Normalize()
{
    float length = sqrtf(x * x + y * y + z * z);
    x = x / length;
    y = y / length;
    z = z / length;
}

float Vec3::Length()
{
    // Euclidean norm
    return sqrtf(x*x + y*y + z*z);
}

Vec3 Vec3::CrossProduct(const Vec3 &input)
{
    Vec3 output;
    output.x = y*input.z - z*input.y;
    output.y = z*input.x - x*input.z;
    output.z = x*input.y - y*input.x;
    return output;
}

Vec3 Vec3::intersectPlane(Vec3 point, Vec3 normal, Vec3 lineEnd)
{
    normal.Normalize();
    float d = -(point * normal);
    float ad = normal * (*this);
    float bd = normal * lineEnd;
    float t = (-d - ad) / (bd - ad);
    Vec3 lineStartToEnd = lineEnd - (*this);
    Vec3 lineToIntersect = lineStartToEnd * t;
    return (*this) + lineToIntersect;
}
