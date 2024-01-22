#include "stdafx.h"

#include "Gun.h"

Gun::Gun(float verticalFOVDeg, float aspectRatio)
{
	m_verticalFOV = verticalFOVDeg * (acosf(-1.0f) / 180.0f);
	m_horizontalFOV = 2.0f * atanf(tanf(m_verticalFOV / 2.0f) * aspectRatio);
	m_yawPerPixel = m_horizontalFOV / APP_VIRTUAL_WIDTH;
	m_pitchPerPixel = m_verticalFOV / APP_VIRTUAL_HEIGHT;
}

void Gun::FireGun(ECS& ecs, EntityDescriptor player)
{
	if (App::IsKeyPressed(VK_LBUTTON) && m_timeUntilReadyToFire <= 0.0f)
	{
		m_timeUntilReadyToFire = m_reloadTime;

		EntityDescriptor newEntityDescriptor = ecs.GetIDs().CreateId();
		if (newEntityDescriptor.isValid())
		{
			ComponentPool<MeshComponent>& meshes = ecs.GetMeshes();
			meshes.Add(newEntityDescriptor, MeshLibrary::ICOSPHERE);

			ComponentPool<TextureComponent>& textures = ecs.GetTextures();
			textures.Add(newEntityDescriptor, m_colour1, m_colour2);

			ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
			transforms.Add(newEntityDescriptor);
			transforms.Get(newEntityDescriptor.id).position = transforms.Get(player.id).position + m_launchDirection * 4.0f;

			ComponentPool<SphereColliderComponent>& spheres = ecs.GetSphereColliders();
			spheres.Add(newEntityDescriptor, 1.0f);

			ecs.GetPhysicsBodies().Add(newEntityDescriptor);
			ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetMass(1.0f);
			ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetGravity(20.0f);
			ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetDamping(.99f);
			ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).AddForce(m_launchDirection * m_force);

			ComponentPool<ProjectileComponent>& projectiles = ecs.GetProjectiles();
			projectiles.Add(newEntityDescriptor, m_damage);

			ComponentPool<LifetimeComponent>& lifetimes = ecs.GetLifetimes();
			lifetimes.Add(newEntityDescriptor, m_projectileLifetime);
		}
	}
}

void Gun::SetLaunchDirection(Camera& camera)
{
	float mouseX, mouseY;
	App::GetMousePos(mouseX, mouseY);

	float yawPixels = mouseX - APP_VIRTUAL_WIDTH / 2.0f;
	float pitchPixels = mouseY - APP_VIRTUAL_HEIGHT / 2.0f;

	m_launchDirection = camera.m_lookDirection;

	Matrix4 yawRotation;
	yawRotation.rotationAxisAngle(camera.up, -m_yawPerPixel * yawPixels);
	Matrix4 pitchRotation;
	pitchRotation.rotationAxisAngle(camera.right, m_pitchPerPixel * pitchPixels);

	m_launchDirection = yawRotation * (pitchRotation * m_launchDirection);

	m_launchDirection.Normalize();
}

void Gun::UpdateTimers(float deltaTime)
{
	m_timeUntilReadyToFire -= deltaTime;
	m_superGunTimeLeft -= deltaTime;
}

void Gun::AddSupergunTime(float time)
{
	if (m_superGunTimeLeft < 0.0f)
	{
		m_superGunTimeLeft = 0.0f;
	}
	m_superGunTimeLeft += time;
}

void Gun::SetGunParameters()
{
	if (m_superGunTimeLeft > 0.0f)
	{
		m_damage = 2;
		m_reloadTime = 60.0f / 600.0f;
		m_force = 6000.0f;
		m_colour1 = Colour(1.0f, 0.75f, 0.75f);
		m_colour2 = Colour(1.0f, 0.1f, 0.6f);
	}
	else
	{
		m_damage = 1;
		m_reloadTime = 60.0f / 200.0f;
		m_force = 4000.0f;
		m_colour1 = Colour(1.0f, 0.1f, 0.6f);
		m_colour2 = Colour(1.0f, 0.75f, 0.75f);
	}
}

void Gun::SetForce(float force)
{
	m_force = force;
}

void Gun::SetDamage(int damage)
{
	m_damage = damage;
}

