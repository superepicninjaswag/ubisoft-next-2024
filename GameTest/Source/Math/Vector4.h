#pragma once


struct Vector4
{
public:
    float x;
    float y;
    float z;
    float w;

    Vector4 operator+(const Vector4& i);
    void operator+=(const Vector4& i);
    Vector4 operator-(const Vector4& i);
    Vector4 operator*(const float& i); // Scalar multiplication
    void operator*=(const float& i);
    float operator*(const Vector4& i); // Dot product
    Vector4 operator^(const Vector4& i);  // Cross product

    void Normalize();
    float Length2D();
    float Length3D();
    Vector4 intersectPlane(Vector4 point, Vector4 normal, Vector4 lineEnd);

    Vector4();
    Vector4(float newX, float newY);               // For 2D purposes
    Vector4(float newX, float newY, float newZ);   // For 3D purposes
};
