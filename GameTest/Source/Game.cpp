//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"
#include "Rendering/MeshLibrary.h"

#include "Systems/Systems.h"


ECS g_ecs;
MeshLibrary g_MeshLibrary;
Renderer g_renderer(g_ecs, g_MeshLibrary);

int limit = 3;
std::default_random_engine rng;
std::uniform_int_distribution<int> dist(1, limit * limit);

// Called before first update. Do any initial setup here.
void Init()
{
	rng.seed(std::random_device{}());
	//*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();

	float gap = 4.0f;
	Colour red(1.0f, 0.0f, 0.0f);
	Colour green(0.0f, 1.0f, 0.0f);
	Colour blue(0.3f, 0.0f, .5f);
	int count = 0;
	for (int i = 0; i < limit; i++)
	{
		for (int j = 0; j < limit; j++)
		{
			EntityDescriptor newEntityDescriptor = g_ecs.GetIDs().CreateId();

			if (newEntityDescriptor.isValid())
			{
				ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
				meshes.Add(newEntityDescriptor, &g_MeshLibrary.cone);

				ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
				textures.Add(newEntityDescriptor, green, blue);

				g_ecs.GetTransforms().Add(newEntityDescriptor);
				g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.x = gap * (i - (limit / 2.0f) + 0.5f);
				g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.y = gap * (j - (limit / 2.0f) + 0.5f);
				g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.z = 50.0f;
			}
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
	float speed = 100.0f;
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

	static int wow = 0;
	if (App::IsKeyPressed('H'))
	{
		int destroyThisOne = (wow++) % (limit * limit);
		ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
		ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
		ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
		EntityDescriptor victim = meshes.MirrorToEntityDescriptor(destroyThisOne);
		std::cout << destroyThisOne << " | " << victim.id << " | " << victim.version << "\n";
		bool well = meshes.Delete(victim);
		bool well2 = textures.Delete(victim);
		bool well3 = transforms.Delete(victim);
		g_ecs.GetIDs().DeleteId(victim);
	}
	else if (g_ecs.GetMeshes().Size() < limit * limit)
	{
		float gap = 4.0f;
		Colour red(1.0f, 0.0f, 0.0f);
		Colour green(0.0f, 1.0f, 0.0f);
		Colour blue(0.3f, 0.0f, .5f);
		EntityDescriptor newEntityDescriptor = g_ecs.GetIDs().CreateId();
		ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
		meshes.Add(newEntityDescriptor, &g_MeshLibrary.cone);

		ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
		textures.Add(newEntityDescriptor, blue, red);

		g_ecs.GetTransforms().Add(newEntityDescriptor);
		g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.x = gap * ((dist(rng) % 10) - (limit / 2.0f) + 0.5f);
		g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.y = gap * ((dist(rng) % 10) - (limit / 2.0f) + 0.5f);
		g_ecs.GetTransforms().Get(newEntityDescriptor.id).v.z = 40.0f;
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