#include "stdafx.h"

#include "GameMath.h"

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
    float length = this->Length();
    x /= length;
    y /= length;
    z /= length;
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

Matrix4x4::Matrix4x4() : a{ 0 }
{

}

void Matrix4x4::identity()
{
    (*this)(0, 0) = 1.0f;
    (*this)(1, 1) = 1.0f;
    (*this)(2, 2) = 1.0f;
    (*this)(3, 3) = 1.0f;
}

void Matrix4x4::rotationX(float r)
{
    (*this)(0,0) = 1.0f;
    (*this)(1,1) = cosf(r);
    (*this)(1,2) = sinf(r);
    (*this)(2,1) = -sinf(r);
    (*this)(2,2) = cosf(r);
    (*this)(3,3) = 1.0f;
}

void Matrix4x4::rotationY(float r)
{
    (*this)(0,0) = cosf(r);
    (*this)(0,2) = sinf(r);
    (*this)(2,0) = -sinf(r);
    (*this)(1,1) = 1.0f;
    (*this)(2,2) = cosf(r);
    (*this)(3,3) = 1.0f;
}

void Matrix4x4::rotationZ(float r)
{
    (*this)(0,0) = cosf(r);
    (*this)(0,1) = sinf(r);
    (*this)(1,0) = -sinf(r);
    (*this)(1,1) = cosf(r);
    (*this)(2,2) = 1.0f;
    (*this)(3,3) = 1.0f;
}

void Matrix4x4::translation(float x, float y, float z)
{
    (*this).identity();
    (*this)(3,0) = x;
    (*this)(3,1) = y;
    (*this)(3,2) = z;
}

float &Matrix4x4::operator()(int row, int col)
{
    return a[row][col];
}

Vec3 Matrix4x4::operator*(Vec3 &i)
{
    Vec3 o;
    o.x = i.x * (*this)(0,0) + i.y * (*this)(1,0) + i.z * (*this)(2,0) + i.w * (*this)(3,0);
    o.y = i.x * (*this)(0,1) + i.y * (*this)(1,1) + i.z * (*this)(2,1) + i.w * (*this)(3,1);
    o.z = i.x * (*this)(0,2) + i.y * (*this)(1,2) + i.z * (*this)(2,2) + i.w * (*this)(3,2);
    o.w = i.x * (*this)(0,3) + i.y * (*this)(1,3) + i.z * (*this)(2,3) + i.w * (*this)(3,3);
    return o;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& i)
{
    Matrix4x4 o;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            o(r, c) = (*this)(r, 0) * i(0, c) + (*this)(r, 1) * i(1, c) + (*this)(r,2) * i(2,c) + (*this)(r,3) * i(3,c);
    return o;
}


