#include "stdafx.h"

#include "MovePlayer.h"

void MovePlayer(ECS &ecs, EntityDescriptor player, Vector4 forward, Vector4 right, float speed, float deltaTime, float maxX, float maxZ)
{
    ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();

    forward.y = 0.0f;
    forward.Normalize();

    right.y = 0.0f;
    right.Normalize();

    if (App::IsKeyPressed('W'))
    {
        float a = deltaTime * speed;
        Vector4 dir = forward * a;
        transforms.Get(player.id).position = transforms.Get(player.id).position + dir;
    }
    if (App::IsKeyPressed('S'))
    {
        transforms.Get(player.id).position = transforms.Get(player.id).position - forward * deltaTime * speed;
    }
    if (App::IsKeyPressed('A'))
    {
        transforms.Get(player.id).position = transforms.Get(player.id).position - right * deltaTime * speed;
    }
    if (App::IsKeyPressed('D'))
    {
        transforms.Get(player.id).position = transforms.Get(player.id).position + right * deltaTime * speed;
    }


    transforms.Get(player.id).position.x = min(transforms.Get(player.id).position.x, maxX);
    transforms.Get(player.id).position.x = max(transforms.Get(player.id).position.x, -maxX);
    transforms.Get(player.id).position.z = min(transforms.Get(player.id).position.z, maxZ);
    transforms.Get(player.id).position.z = max(transforms.Get(player.id).position.z, -maxZ);
}
