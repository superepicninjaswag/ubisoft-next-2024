//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"
#include "Rendering/MeshAssetManager.h"

#include "Systems/Systems.h"


ECS g_ecs;
MeshAssetManager g_meshAssetManager;
Renderer g_renderer(g_ecs, g_meshAssetManager);


// Called before first update. Do any initial setup here.
void Init()
{
	/*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();
	g_meshAssetManager.LoadMeshAsset("cone");
	g_meshAssetManager.LoadMeshAsset("cube");
	g_meshAssetManager.LoadMeshAsset("cylinder");
	g_meshAssetManager.LoadMeshAsset("icosphere");
	g_meshAssetManager.LoadMeshAsset("monkey");
	g_meshAssetManager.LoadMeshAsset("plane");
	g_meshAssetManager.LoadMeshAsset("torus");
	g_meshAssetManager.LoadMeshAsset("uvsphere");

	int limit = 8;
	float gap = 4.0f;
	Colour red(1.0f, 0.0f, 0.0f);
	Colour green(0.0f, 1.0f, 0.0f);
	Colour blue(0.3f, 0.0f, .5f);
	for (int i = 0; i < limit; i++)
	{
		for (int j = 0; j < limit; j++)
		{
			int newEntity = g_ecs.GetIDs().CreateId();
			ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
			meshes.Add(newEntity, "cube");
			ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
			if (j == 0 && i == 0)
			{
				textures.Add(newEntity, blue);
			}
			else
			{
				textures.Add(newEntity, green, blue);
			}
			g_ecs.GetTransforms().Add(newEntity);
			g_ecs.GetTransforms().Get(newEntity)->v.x = gap*(i - (limit / 2.0f) + 0.5f);
			g_ecs.GetTransforms().Get(newEntity)->v.y = gap*(j - (limit / 2.0f) + 0.5f);
			g_ecs.GetTransforms().Get(newEntity)->v.z = 50.0f;
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
	g_renderer.theta += 1.0f * deltaTime;

	// Move camera
	float speed = 40.0f;
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
		g_renderer.camera.y += speed * deltaTime;
	}
	if (App::IsKeyPressed(VK_SHIFT))
	{
		g_renderer.camera.y -= speed * deltaTime;
	}
	if (App::IsKeyPressed(VK_LEFT))
	{
		g_renderer.yaw -= deltaTime;
	}
	if (App::IsKeyPressed(VK_RIGHT))
	{
		g_renderer.yaw += deltaTime;
	}
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