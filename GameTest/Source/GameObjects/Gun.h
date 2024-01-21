#pragma once

#include "../Math/Vector4.h"
#include "../ECS/ECS.h"
#include "../Rendering/Camera.h"
#include "../Rendering/MeshLibrary.h"


class Gun
{
private:
    int m_damage;
    float m_verticalFOV;
    float m_horizontalFOV;
    float m_yawPerPixel;
    float m_pitchPerPixel;
    Vector4 m_launchDirection;
    float m_force = 1000.0f;
    float m_reloadTime = 0.5f;
    float m_timeUntilReadyToFire = 0.0f;
    const int m_projectileLifetime = 600;

public:
    Gun(int damage, float verticalFOVDeg, float aspectRatio);
    void FireGun(ECS& ecs, EntityDescriptor player);
    void SetLaunchDirection(Camera& camera);
    void UpdateReloadTimer(float deltaTime);
    void SetForce(float force);
    void SetDamage(int damage);
};