//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"

#include "Systems/Systems.h"
#include "Util/GetModelNumber.h"


ECS g_ecs;
Renderer g_renderer;


// Called before first update. Do any initial setup here.
void Init()
{
	//*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();
	MeshResourceObjLoader(g_ecs);

	int limit = 1;
	float gap = 4.0f;
	for (int i = 0; i < limit; i++)
	{
		for (int j = 0; j < limit; j++)
		{
			int newEntity = g_ecs.GetIDs().CreateId();
			Pool<MeshComponent>& meshes = g_ecs.GetMeshes();
			meshes.Add(newEntity, GetModelNumber(g_ecs.GetMeshResources(), "monkey"));
			g_ecs.GetTransforms().Add(newEntity);
			g_ecs.GetTransforms().Get(newEntity)->v.x = gap*(i - (limit / 2.0f) + 0.5f);
			g_ecs.GetTransforms().Get(newEntity)->v.y = gap*(j - (limit / 2.0f) + 0.5f);
			g_ecs.GetTransforms().Get(newEntity)->v.z = 3.0f;
		}
	}
}


// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	// Convert deltaTime from ms to seconds
	deltaTime = deltaTime * 1.0f / 1000.0f;

	// Set up rotation matrices
	//g_renderer.theta += 1.0f * deltaTime;

	// Move camera
	float speed = .5f;
	if (App::IsKeyPressed('W'))
	{
		g_renderer.camera = g_renderer.camera + g_renderer.cameraLookDirection * deltaTime * speed;
	}
	if (App::IsKeyPressed('S'))
	{
		g_renderer.camera = g_renderer.camera - g_renderer.cameraLookDirection * deltaTime * speed;
	}
	if (App::IsKeyPressed('A'))
	{
		g_renderer.camera = g_renderer.camera - g_renderer.right * deltaTime * speed;
	}
	if (App::IsKeyPressed('D'))
	{
		g_renderer.camera = g_renderer.camera + g_renderer.right * deltaTime * speed;
	}
	if (App::IsKeyPressed(VK_SPACE))
	{
		g_renderer.camera.y += 5.0f * deltaTime;
	}
	if (App::IsKeyPressed(VK_SHIFT))
	{
		g_renderer.camera.y -= 5.0f * deltaTime;
	}
	if (App::IsKeyPressed(VK_LEFT))
	{
		g_renderer.yaw += 2.0f * deltaTime;
	}
	if (App::IsKeyPressed(VK_RIGHT))
	{
		g_renderer.yaw -= 2.0f * deltaTime;
	}
}


// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
void Render()
{
	Pool<MeshComponent>& meshes = g_ecs.GetMeshes();
	Pool<MeshResourceComponent>& meshResources = g_ecs.GetMeshResources();
	Pool<TransformComponent>& transforms = g_ecs.GetTransforms();

	g_renderer.Render(meshes, transforms, meshResources);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	return;
}