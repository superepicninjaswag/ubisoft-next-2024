#pragma once


struct Vector4
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Vector4 operator+(const Vector4&i);
    Vector4 operator-(const Vector4&i);
    Vector4 operator*(const float &i); // Scalar multiplication
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
