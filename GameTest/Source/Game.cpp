//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"
#include "./Rendering/MeshLibrary.h"

#include "./Systems/Systems.h"
#include "./Physics/Physics.h"

#include "./GameObjects/GameObjects.h"
#include "./Scenery/Scenery.h"


// Engine related
ECS g_ecs;
MeshLibrary g_MeshLibrary;
Camera g_camera;
Renderer g_renderer(g_ecs, g_MeshLibrary, g_camera);

// Scenery
Ground g_ground(5.0f, 30, 90);
// TODO: clouds might be nice

// Player
EntityDescriptor g_player = g_ecs.GetIDs().CreateId();
Gun g_gun(g_renderer.GetFOV(), g_renderer.GetAspectRatio());

// AI manager?

// Called before first update. Do any initial setup here.
void Init()
{
	/*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();

	// Setup player entity
	ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
	transforms.Add(g_player);
	transforms.Get(g_player.id).position.x = 0.0f;
	transforms.Get(g_player.id).position.y = 16.0f;
	transforms.Get(g_player.id).position.z = -450.0f;
	ComponentPool<SphereColliderComponent>& spheres = g_ecs.GetSphereColliders();
	spheres.Add(g_player, 1.0f);

	// Setup scenery
	g_ground.GenerateTiles(g_ecs);


	// Enemy spawner
	int limit = 1;
	for (int i = 0; i < limit; i++)
	{
		EntityDescriptor newEntityDescriptor = g_ecs.GetIDs().CreateId();
		if (newEntityDescriptor.isValid())
		{
			ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
			meshes.Add(newEntityDescriptor, MeshLibrary::CUBE);

			ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
			textures.Add(newEntityDescriptor, Colour(1.0f, 0.0f, 0.0f));

			ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
			transforms.Add(newEntityDescriptor);
			transforms.Get(newEntityDescriptor.id).position.x = i - limit/2.0f;
			transforms.Get(newEntityDescriptor.id).position.y = 25.0f;
			transforms.Get(newEntityDescriptor.id).position.z = -300.0f;

			ComponentPool<SphereColliderComponent>& spheres = g_ecs.GetSphereColliders();
			spheres.Add(newEntityDescriptor, 1.0f);

			ComponentPool<SinWaveAIComponent>& SinWaveAIs = g_ecs.GetSinWaveAIs();
			SinWaveAIs.Add(newEntityDescriptor);

			g_ecs.GetEnemies().Add(newEntityDescriptor);
			g_ecs.GetHealth().Add(newEntityDescriptor);
		}
	}
}


// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	deltaTime = deltaTime / 1000.0f; // I like seconds instead of milliseconds

	g_gun.SetSupergunPowers();
	g_gun.SetLaunchDirection(g_camera);
	g_gun.UpdateTimers(deltaTime);
	g_gun.FireGun(g_ecs, g_player);

	MovePlayer(g_ecs, g_player, g_camera.forward, g_camera.right, 30.0f, deltaTime, g_ground);
	UpdateLifetimes(g_ecs);
	DeleteDeadEntities(g_ecs);

	AnimateSinWaveAI(g_ecs, g_player);
	AnimateParticles(g_ecs);
	UpdatePhysicsBodies(g_ecs, deltaTime);
	
	ResolveBulletGroundCollisions(g_ecs, g_ground);
	ResolveBulletEnemyCollisions(g_ecs);

	g_camera.UpdatePosition(g_ecs.GetTransforms().Get(g_player.id).position);
	g_camera.UpdatePitchAndYaw(deltaTime);
}


// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
void Render()
{
	g_renderer.Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	g_renderer.Shutdown();
}