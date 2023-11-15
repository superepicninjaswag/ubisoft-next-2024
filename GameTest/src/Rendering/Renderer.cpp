#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() {}

void Renderer::Init()
{   
    lightDirection.x = 0.0f;
    lightDirection.y = 0.0f;
    lightDirection.z = -1.0f;
    lightDirection.Normalize();

    (*this).setProjectMatrix(fovDeg, aspectRatio, zNear, zFar);

    camera.x = 0.0f;
    camera.y = 0.0f;
    camera.z = 0.0f;
}

void Renderer::Render(Pool<MeshComponent> &meshes, Pool<TransformComponent> &transforms, Pool<MeshResourceComponent> &meshResources)
{
    std::vector<Face> facesToRender(512);
    for (int i = 0; i < meshes.Size(); i++)
    {
        int meshResourceId = meshes._dense[i].meshResourceId;
        int entityId = meshes.MirrorIdToEntityId(i);
        
        Matrix4x4 worldMatrix, rX, rY, rZ, translation;
        
        // Rotation matrices
        rX.rotationX(theta);
        rY.rotationY(theta * 0.5f);
        rZ.rotationZ(theta * 0.25f);

        // Translation matrix
        Vec3 position = transforms.Get(entityId)->v;
        translation.translation(position.x, position.y, position.z);

        // World matrix
        worldMatrix.identity();
        worldMatrix = worldMatrix * rX * rY * rZ * translation;

        for(auto f : meshResources.Get(meshResourceId)->faces)
        {
            Face faceTransformed;
            for (int i = 0; i < 3; i++)
            {
                faceTransformed.points[i] = worldMatrix * f.points[i];
            }

            // Calculate face normal
            Vec3 line1 = faceTransformed.points[1] - faceTransformed.points[0];
            Vec3 line2 = faceTransformed.points[2] - faceTransformed.points[0];
            Vec3 normal = line1.CrossProduct(line2);
            normal.Normalize();

            // Normal projection onto line from camera to face
            float faceNormalProjectionOntoCameraRay =  normal * (faceTransformed.points[0] - camera);

            /*
            If the projection of the face normal is negative (relative
            to a ray from the camera), then we know the face is pointing
            away from the camera
            */
            if (faceNormalProjectionOntoCameraRay < 0.0f)
            {
                // Illumination
                float dp = normal * lightDirection;

                // Project the face into screen space
                Face faceProjected;
                for (int i = 0; i < 3; i++)
                {
                    faceProjected.points[i] = projectionMatrix * faceTransformed.points[i];
                    faceProjected.points[i] = faceProjected.points[i] * (1 / faceProjected.points[i].w);
                }

                // Scale from normalized to screen size
                for (int i = 0; i < 3; i++)
                {
                    faceProjected.points[i].x = (faceProjected.points[i].x + 1) * 0.5 * SCREEN_WIDTH;
                    faceProjected.points[i].y = (faceProjected.points[i].y + 1) * 0.5 * SCREEN_HEIGHT;
                }

                // Apply lighting
                faceProjected.colour.r = dp / 2 + faceProjected.colour.r / 2;
                faceProjected.colour.g = dp / 2 + faceProjected.colour.g / 2;
                faceProjected.colour.b = dp / 2 + faceProjected.colour.b / 2;

                // Add to render queue
                facesToRender.push_back(faceProjected);
            }
        }
    }

    std::sort(facesToRender.begin(), facesToRender.end());
    for(auto f : facesToRender)
    {
        drawTriangle(f, f.colour);
    }
}

void Renderer::setProjectMatrix(float fovDeg, float aspectRatio, float zNear, float zFar)
{
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float fovRad = 1.0f / tanf(fovDeg * 0.5f / 180.0f * pi);
    projectionMatrix(0, 0) = aspectRatio * fovRad;
    projectionMatrix(1, 1) = fovRad;
    projectionMatrix(2, 2) = zFar / (zFar - zNear);
    projectionMatrix(3, 2) = (-zFar * zNear) / (zFar - zNear);
    projectionMatrix(2, 3) = 1.0f;
    projectionMatrix(3, 3) = 0.0f;
}
