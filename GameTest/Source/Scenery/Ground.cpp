#include "stdafx.h"

#include "Ground.h"

Ground::Ground(float scale, int width, int length) : m_scale(scale), m_width(width), m_length(length) {}

void Ground::GenerateTiles(ECS& ecs)
{
	const float GAP = 2.0f * m_scale;
	for (int i = 0; i < m_length; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			Colour brown(0.54509803921f, 0.27058823529f, 0.07450980392f);
			Colour green(0.2f, 1.0f, 0.2f);
			EntityDescriptor newEntityDescriptor = ecs.GetIDs().CreateId();
			if (newEntityDescriptor.isValid())
			{
				ComponentPool<MeshComponent>& meshes = ecs.GetMeshes();
				meshes.Add(newEntityDescriptor, MeshLibrary::PLANE);

				ComponentPool<TextureComponent>& textures = ecs.GetTextures();
				textures.Add(newEntityDescriptor, green, brown);

				ComponentPool<TransformComponent>& transforms = ecs.GetTransforms();
				transforms.Add(newEntityDescriptor);
				transforms.Get(newEntityDescriptor.id).position.x = GAP * (j - (m_width / 2.0f) + 0.5f);
				transforms.Get(newEntityDescriptor.id).position.z = GAP * (i - (m_length / 2.0f) + 0.5f);
				// By scaling we don't need as many entities/tiles
				transforms.Get(newEntityDescriptor.id).scale.x = m_scale;
				transforms.Get(newEntityDescriptor.id).scale.z = m_scale;
			}
		}
	}
}

float Ground::GetMaxX()
{
	return (m_width * 2 * m_scale) / 2.0f;;
}

float Ground::GetMaxZ()
{
	return (m_length * 2 * m_scale) / 2.0f;;
}
