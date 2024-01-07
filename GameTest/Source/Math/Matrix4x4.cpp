#include "stdafx.h"

#include "Matrix4x4.h"

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
    (*this)(0, 0) = 1.0f;
    (*this)(1, 1) = cosf(r);
    (*this)(1, 2) = sinf(r);
    (*this)(2, 1) = -sinf(r);
    (*this)(2, 2) = cosf(r);
    (*this)(3, 3) = 1.0f;
}

void Matrix4x4::rotationY(float r)
{
    (*this)(0, 0) = cosf(r);
    (*this)(0, 2) = sinf(r);
    (*this)(2, 0) = -sinf(r);
    (*this)(1, 1) = 1.0f;
    (*this)(2, 2) = cosf(r);
    (*this)(3, 3) = 1.0f;
}

void Matrix4x4::rotationZ(float r)
{
    (*this)(0, 0) = cosf(r);
    (*this)(0, 1) = sinf(r);
    (*this)(1, 0) = -sinf(r);
    (*this)(1, 1) = cosf(r);
    (*this)(2, 2) = 1.0f;
    (*this)(3, 3) = 1.0f;
}

void Matrix4x4::translation(float x, float y, float z)
{
    (*this).identity();
    a[3][0] = x;
    a[3][1] = y;
    a[3][2] = z;
}

float& Matrix4x4::operator()(int row, int col)
{
    return a[row][col];
}

Vec3 Matrix4x4::operator*(Vec3& i)
{
    Vec3 o;
    o.x = i.x * a[0][0] + i.y * a[1][0] + i.z * a[2][0] + i.w * a[3][0];
    o.y = i.x * a[0][1] + i.y * a[1][1] + i.z * a[2][1] + i.w * a[3][1];
    o.z = i.x * a[0][2] + i.y * a[1][2] + i.z * a[2][2] + i.w * a[3][2];
    o.w = i.x * a[0][3] + i.y * a[1][3] + i.z * a[2][3] + i.w * a[3][3];
    return o;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& i)
{
    Matrix4x4 output;
    for (int column = 0; column < 4; column++)
        for (int row = 0; row < 4; row++)
            output(row, column) = a[row][0] * i(0, column) + a[row][1] * i(1, column) + a[row][2] * i(2, column) + a[row][3] * i(3, column);
    return output;
}