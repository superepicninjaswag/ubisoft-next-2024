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
	g_ground.GenerateTiles(g_ecs);

	// Setup player entity
	ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
	transforms.Add(g_player);
	transforms.Get(g_player.id).position.x = 0.0f;
	transforms.Get(g_player.id).position.y = 16.0f;
	transforms.Get(g_player.id).position.z = -450.0f;
	ComponentPool<SphereColliderComponent>& spheres = g_ecs.GetSphereColliders();
	spheres.Add(g_player, 1.0f);
}


// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	deltaTime = deltaTime / 1000.0f; // I like seconds instead of milliseconds

	g_gun.SetGunParameters();
	g_gun.SetLaunchDirection(g_camera);
	g_gun.UpdateTimers(deltaTime);
	g_gun.FireGun(g_ecs, g_player);

	MovePlayer(g_ecs, g_player, g_camera.forward, g_camera.right, 50.0f, deltaTime, g_ground);
	UpdateLifetimes(g_ecs);
	DeleteDeadEntities(g_ecs);

	AnimateDodgingAI(g_ecs, g_player, g_gun);
	AnimateSinWaveAI(g_ecs, g_player);
	AnimateParticles(g_ecs);
	UpdatePhysicsBodies(g_ecs, deltaTime);
	
	ResolveBulletGroundCollisions(g_ecs, g_ground);
	ResolveBulletEnemyCollisions(g_ecs);

	g_camera.UpdatePosition(g_ecs.GetTransforms().Get(g_player.id).position);
	g_camera.UpdatePitchAndYaw(deltaTime);

	ResolvePlayerPowerupCollisions(g_ecs, g_ground, g_gun, g_player);
	PowerupSpawner(g_ecs, g_ground);
	EnemySpawner(g_ecs, deltaTime, g_ground);
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