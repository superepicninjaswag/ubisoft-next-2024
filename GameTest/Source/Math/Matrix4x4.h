#pragma once

#include "Matrix4x4.h"
#include "Vec3.h"

class Matrix4x4
{
private:
    float a[4][4];
public:
    Matrix4x4();
    void identity();
    void rotationX(float r);
    void rotationY(float r);
    void rotationZ(float r);
    void translation(float x, float y, float z);

    float& operator()(int row, int col);
    Vec3 operator*(Vec3& i);
    Matrix4x4 operator*(Matrix4x4& i);
};