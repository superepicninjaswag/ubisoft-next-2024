#pragma once

#include "../Math/Vector4.h"
#include "../ECS/ECS.h"
#include "../Rendering/Camera.h"
#include "../Rendering/MeshLibrary.h"


class Gun
{
private:
    int m_damage = 1;
    float m_verticalFOV;
    float m_horizontalFOV;
    float m_yawPerPixel;
    float m_pitchPerPixel;
    Vector4 m_launchDirection;
    float m_force = 3000.0f;
    float m_roundsPerMin;
    float m_reloadTime = 60.0f / m_roundsPerMin;
    float m_timeUntilReadyToFire = 0.0f;
    const int m_projectileLifetime = 600;
    float m_superGunTimeLeft = 0.0f;
    Colour m_colour1;
    Colour m_colour2;

public:
    Gun(float verticalFOVDeg, float aspectRatio);
    void FireGun(ECS& ecs, EntityDescriptor player);
    void SetLaunchDirection(Camera& camera);
    void SetForce(float force);
    void SetDamage(int damage);
    void UpdateTimers(float deltaTime);
    void AddSupergunTime(float time);
    void SetSupergunPowers();
};