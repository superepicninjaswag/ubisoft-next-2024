//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
#include "..\App\app.h"
#include <windows.h> 
#include <math.h>

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"

#include "Systems/Systems.h"
#include "Util/GetModelNumber.h"


ECS g_ecs;
Renderer g_renderer;


// Called before first update. Do any initial setup here.
void Init()
{
	/*
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	*/

	g_renderer.Init();
	MeshResourceObjLoader(g_ecs);

	int limit = 64;
	for (int i = 0; i < limit; i++)
	{
		for (int j = 0; j < limit; j++)
		{
			int newEntity = g_ecs.GetIDs().CreateId();
			Pool<MeshComponent>& meshes = g_ecs.GetMeshes();
			meshes.Add(newEntity, GetModelNumber(g_ecs.GetMeshResources(), "cube"));
			g_ecs.GetTransforms().Add(newEntity);
			g_ecs.GetTransforms().Get(newEntity)->v.x = 4 * (i - limit / 2);
			g_ecs.GetTransforms().Get(newEntity)->v.y = 4 * (j - limit / 2);
			g_ecs.GetTransforms().Get(newEntity)->v.z = 200;
		}
	}
}


// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	// Set up rotation matrices
	g_renderer.theta += 0.001f * deltaTime;
}


// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
void Render()
{
	// RENDER ALL THINGS THAT NEED RENDERING
	// TODO: Have each object that is rendered set the colour

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