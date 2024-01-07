#pragma once


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
    Vec3 intersectPlane(Vec3 point, Vec3 normal, Vec3 lineEnd);

    Vec3();
    Vec3(float x, float y, float z);
};
