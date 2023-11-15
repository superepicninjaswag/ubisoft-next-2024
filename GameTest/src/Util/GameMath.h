#pragma once

#include <cmath>

struct Vec3
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
    Vec3 operator+(const Vec3 &i);
    Vec3 operator-(const Vec3 &i);
    Vec3 operator*(const float &i); // Scalar multiplication
    float operator*(const Vec3 &i); // Dot product
    void Normalize();
    float Length();
    Vec3 CrossProduct(const Vec3 &i);

    Vec3();
    Vec3(float x, float y, float z);
};

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

    float &operator()(int row, int col);
    Vec3 operator*(Vec3 &i);
    Matrix4x4 operator*(Matrix4x4& i);
};