#include "stdafx.h"

#include "Vector4.h"


Vector4::Vector4()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

Vector4::Vector4(float newX, float newY)
{
    x = newX;
    y = newY;
    z = 0.0f;
    w = 0.0f;
}

Vector4::Vector4(float newX, float newY, float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
    w = 1.0f;
}

Vector4 Vector4::operator+(const Vector4 &i)
{
    Vector4 o;
    o.x = x + i.x;
    o.y = y + i.y;
    o.z = z + i.z;
    return o;
}

Vector4 Vector4::operator-(const Vector4 &i)
{
    Vector4 o;
    o.x = x - i.x;
    o.y = y - i.y;
    o.z = z - i.z;
    return o;
}

float Vector4::operator*(const Vector4 &i)
{
    return x*i.x + y*i.y + z*i.z;
}

Vector4 Vector4::operator*(const float &i)
{
    Vector4 o;
    o.x = x * i;
    o.y = y * i;
    o.z = z * i;
    return o;
}

Vector4 Vector4::operator^(const Vector4& i)
{
    Vector4 o;
    o.x = y * i.z - z * i.y;
    o.y = z * i.x - x * i.z;
    o.z = x * i.y - y * i.x;
    return o;
}

void Vector4::Normalize()
{
    float length = sqrtf(x * x + y * y + z * z);
    x = x / length;
    y = y / length;
    z = z / length;
}

float Vector4::Length2D()
{
    return sqrtf(x * x + y * y);
}

float Vector4::Length3D()
{
    return sqrtf(x * x + y * y + z * z);
}

Vector4 Vector4::intersectPlane(Vector4 point, Vector4 normal, Vector4 lineEnd)
{
    normal.Normalize();
    float d = -(point * normal);
    float ad = normal * (*this);
    float bd = normal * lineEnd;
    float t = (-d - ad) / (bd - ad);
    Vector4 lineStartToEnd = lineEnd - (*this);
    Vector4 lineToIntersect = lineStartToEnd * t;
    return (*this) + lineToIntersect;
}
