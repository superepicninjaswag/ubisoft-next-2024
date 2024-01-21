//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"

#include "./ECS/ECS.h"
#include "./Rendering/Renderer.h"
#include "Rendering/MeshLibrary.h"

#include "Systems/Systems.h"
#include "Physics/Physics.h"


ECS g_ecs;
MeshLibrary g_MeshLibrary;
Camera mainCamera;
Renderer g_renderer(g_ecs, g_MeshLibrary, mainCamera);
std::vector<Contact> contactQueue;
EntityDescriptor player = g_ecs.GetIDs().CreateId();
float maxX, maxZ;


// Called before first update. Do any initial setup here.
void Init()
{
	/*	If I needed a console for debugging with print lines
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	//*/

	g_renderer.Init();

	// Create Player
	ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
	transforms.Add(player);
	transforms.Get(player.id).position.y = 16.0f;
	ComponentPool<SphereColliderComponent>& spheres = g_ecs.GetSphereColliders();
	spheres.Add(player, 1.0f);

	// Create Ground
	const int ENVIRONMENT_LENGTH = 90;
	const int ENVIRONMENT_WIDTH = 30;
	const int GROUND_PLANE_SCALE = 5;
	const float GAP = 2 * GROUND_PLANE_SCALE;
	for (int i = 0; i < ENVIRONMENT_LENGTH; i++)
	{
		for (int j = 0; j < ENVIRONMENT_WIDTH; j++)
		{
			Colour brown(0.54509803921f, 0.27058823529f, 0.07450980392f);
			Colour white(1.0f, 1.0f, 1.0f);
			Colour blue(0.3f, 0.0f, .5f);
			EntityDescriptor newEntityDescriptor = g_ecs.GetIDs().CreateId();
			if (newEntityDescriptor.isValid())
			{
				ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
				meshes.Add(newEntityDescriptor, MeshLibrary::PLANE);

				ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
				textures.Add(newEntityDescriptor, white, brown);

				ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
				transforms.Add(newEntityDescriptor);
				transforms.Get(newEntityDescriptor.id).position.x = GAP * (j - (ENVIRONMENT_WIDTH / 2.0f) + 0.5f);
				transforms.Get(newEntityDescriptor.id).position.z = GAP * (i - (ENVIRONMENT_LENGTH / 2.0f) + 0.5f);
				// By scaling we don't need as many entities/tiles
				transforms.Get(newEntityDescriptor.id).scale.x = GROUND_PLANE_SCALE;
				transforms.Get(newEntityDescriptor.id).scale.z = GROUND_PLANE_SCALE;
			}
		}
	}
	maxX = (ENVIRONMENT_WIDTH * GAP) / 2.0f;
	maxZ = (ENVIRONMENT_LENGTH * GAP) / 2.0f;

	// Create walls


	// Create ball
	EntityDescriptor newEntityDescriptor = g_ecs.GetIDs().CreateId();
	if (newEntityDescriptor.isValid())
	{
		Colour white(1.0f, 1.0f, 1.0f);
		Colour purple(0.3f, 0.0f, .5f);

		ComponentPool<MeshComponent>& meshes = g_ecs.GetMeshes();
		meshes.Add(newEntityDescriptor, MeshLibrary::ICOSPHERE);

		ComponentPool<TextureComponent>& textures = g_ecs.GetTextures();
		textures.Add(newEntityDescriptor, white, purple);

		ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
		transforms.Add(newEntityDescriptor);
		transforms.Get(newEntityDescriptor.id).position.z = 15.0f;
		transforms.Get(newEntityDescriptor.id).position.y = 15.0f;

		ComponentPool<SphereColliderComponent>& spheres = g_ecs.GetSphereColliders();
		spheres.Add(newEntityDescriptor, 1.0f);

		g_ecs.GetPhysicsBodies().Add(newEntityDescriptor);
		g_ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetMass(1.0f);
		g_ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetGravity(10.0f);
		g_ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).SetDamping(.99f);
		g_ecs.GetPhysicsBodies().Get(newEntityDescriptor.id).AddForce(Vector4(300.0, 0.0f, 0.0));
	}
}


// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
void Update(float deltaTime)
{
	// I like seconds instead of milliseconds
	deltaTime = deltaTime * 1.0f / 1000.0f;

	MovePlayer(g_ecs, player, mainCamera.forward, mainCamera.right, 40.0f, deltaTime, maxX, maxZ);
	AnimateParticles(g_ecs);

	ComponentPool<PhysicsBodyComponent>& bodies = g_ecs.GetPhysicsBodies();
	ComponentPool<TransformComponent>& transforms = g_ecs.GetTransforms();
	for (int i = 0; i < bodies.Size(); i++)
	{
		EntityDescriptor target = bodies.MirrorToEntityDescriptor(i);
		bodies.Get(target.id).Integrate(deltaTime, transforms.Get(target.id).position);
	}
	
	
	DetectCollisions(g_ecs, contactQueue);
	ResolveCollisions(g_ecs, contactQueue);
	mainCamera.UpdatePosition(g_ecs.GetTransforms().Get(player.id).position);
	mainCamera.UpdatePitchAndYaw(deltaTime);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
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