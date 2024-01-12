#pragma once

#include "Matrix4.h"
#include "Vector4.h"

class Matrix4
{
private:
    float a[4][4];
public:
    Matrix4();
    void identity();
    void rotationX(const float r);
    void rotationY(const float r);
    void rotationZ(const float r);
    void translation(const float x, const float y, const float z);

    float& operator()(const int row, const int col);
    Vector4 operator*(const Vector4& i);
    Matrix4 operator*(Matrix4& i);
};