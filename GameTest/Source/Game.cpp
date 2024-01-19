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


// Called before first update. Do any initial setup here.
void Init()
{
	/*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();

	int limit = 1;
	float gap = 8.0f;
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
				meshes.Add(newEntityDescriptor, MeshCode::CUBE);

				ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
				textures.Add(newEntityDescriptor, green, blue);

				ComponentPool<ParticleComponent>& particles = g_ecs.GetParticles();
				particles.Add(newEntityDescriptor, 1.0f, 1.0f, 1.0f, 2000);

				ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
				transforms.Add(newEntityDescriptor);
				transforms.Get(newEntityDescriptor.id).position.x = gap * (i - (limit / 2.0f) + 0.5f);
				transforms.Get(newEntityDescriptor.id).position.y = gap * (j - (limit / 2.0f) + 0.5f);
				transforms.Get(newEntityDescriptor.id).position.z = 50.0f;
				//g_ecs.GetTransforms().Get(newEntityDescriptor.id).scale.x = 10.0f;
				//g_ecs.GetTransforms().Get(newEntityDescriptor.id).scale.y = 10.0f;
				//g_ecs.GetTransforms().Get(newEntityDescriptor.id).scale.z = 10.0f;
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
		g_renderer.yaw -= 1.5f * deltaTime;
	}
	if (App::IsKeyPressed(VK_RIGHT))
	{
		g_renderer.yaw += 1.5f * deltaTime;
	}

	for (int i = 0; i < g_ecs.GetTransforms().Size();i++)
	{
		EntityDescriptor targetEntity = g_ecs.GetTransforms().MirrorToEntityDescriptor(i);
		g_ecs.GetTransforms().Get(targetEntity.id).rotation.y += deltaTime;
	}

	AnimateParticles(g_ecs);
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