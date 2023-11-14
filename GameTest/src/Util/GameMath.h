#pragma once

#include <cmath>

struct Vec3
{
public:
    float x, y, z;
    Vec3 operator+(const Vec3 &i);
    Vec3 operator-(const Vec3 &i);
    Vec3 operator*(const float &i);
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
    float &operator()(int row, int col);
    Vec3 operator*(const Vec3 &i);
};