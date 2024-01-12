#include "stdafx.h"

#include "Matrix4.h"

Matrix4::Matrix4() : a{ 0 }
{

}

void Matrix4::identity()
{
    (*this)(0, 0) = 1.0f;
    (*this)(1, 1) = 1.0f;
    (*this)(2, 2) = 1.0f;
    (*this)(3, 3) = 1.0f;
}

void Matrix4::rotationX(const float r)
{
    (*this)(0, 0) = 1.0f;
    (*this)(1, 1) = cosf(r);
    (*this)(1, 2) = -sinf(r);
    (*this)(2, 1) = sinf(r);
    (*this)(2, 2) = cosf(r);
    (*this)(3, 3) = 1.0f;
}

void Matrix4::rotationY(const float r)
{
    (*this)(0, 0) = cosf(r);
    (*this)(0, 2) = sinf(r);
    (*this)(1, 1) = 1.0f;
    (*this)(2, 0) = -sinf(r);
    (*this)(2, 2) = cosf(r);
    (*this)(3, 3) = 1.0f;
}

void Matrix4::rotationZ(const float r)
{
    (*this)(0, 0) = cosf(r);
    (*this)(0, 1) = -sinf(r);
    (*this)(1, 0) = sinf(r);
    (*this)(1, 1) = cosf(r);
    (*this)(2, 2) = 1.0f;
    (*this)(3, 3) = 1.0f;
}

void Matrix4::translation(const float x, const float y, const float z)
{
    (*this).identity();
    a[0][3] = x;
    a[1][3] = y;
    a[2][3] = z;
}

inline float& Matrix4::operator()(const int row, const int col)
{
    return a[row][col];
}

Vector4 Matrix4::operator*(const Vector4& i)
{
    Vector4 o;
    o.x = i.x * a[0][0] + i.y * a[0][1] + i.z * a[0][2] + i.w * a[0][3];
    o.y = i.x * a[1][0] + i.y * a[1][1] + i.z * a[1][2] + i.w * a[1][3];
    o.z = i.x * a[2][0] + i.y * a[2][1] + i.z * a[2][2] + i.w * a[2][3];
    o.w = i.x * a[3][0] + i.y * a[3][1] + i.z * a[3][2] + i.w * a[3][3];
    return o;
}

Matrix4 Matrix4::operator*(Matrix4& i)
{
    Matrix4 output;
    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            output(row, column) = a[row][0] * i(0, column) + a[row][1] * i(1, column) + a[row][2] * i(2, column) + a[row][3] * i(3, column);
    return output;
}