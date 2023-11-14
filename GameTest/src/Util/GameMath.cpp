#include "stdafx.h"

#include "GameMath.h"

Vec3::Vec3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
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

Matrix4x4::Matrix4x4() : a{ 0 }
{

}

float &Matrix4x4::operator()(int row, int col)
{
    return a[row][col];
}

Vec3 Matrix4x4::operator*(const Vec3 &i)
{
    Vec3 o;
    o.x = i.x * (*this)(0,0) + i.y * (*this)(1,0) + i.z * (*this)(2,0) + (*this)(3,0);
    o.y = i.x * (*this)(0,1) + i.y * (*this)(1,1) + i.z * (*this)(2,1) + (*this)(3,1);
    o.z = i.x * (*this)(0,2) + i.y * (*this)(1,2) + i.z * (*this)(2,2) + (*this)(3,2);
    float w = i.x * (*this)(0,3) + i.y * (*this)(1,3) + i.z * (*this)(2,3) + (*this)(3,3);

    if (w != 0)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }

    return o;
}
