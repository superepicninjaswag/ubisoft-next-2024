#include "stdafx.h"

#include "Renderer.h"

Matrix4x4 g_projectionMatrix;

Renderer::Renderer() {}

void Renderer::Init()
{   
    lightDirection.x = 0.0f;
    lightDirection.y = 0.0f;
    lightDirection.z = -1.0f;
    lightDirection.Normalize();

    g_projectionMatrix(0, 0) = aspectRatio * fovRad;
    g_projectionMatrix(1, 1) = fovRad;
    g_projectionMatrix(2, 2) = zFar / (zFar - zNear);
    g_projectionMatrix(3, 2) = (-zFar * zNear) / (zFar - zNear);
    g_projectionMatrix(2, 3) = 1.0f;
    g_projectionMatrix(3, 3) = 0.0f;

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
        for(auto f : meshResources.Get(meshResourceId)->faces)
        {
            // Rotate
            Face faceRotatedZ, faceRotatedZX;

            // Rotate in Z-Axis
            faceRotatedZ = f;
            faceRotatedZ.points[0] = matRotZ * f.points[0];
            faceRotatedZ.points[1] = matRotZ * f.points[1];
            faceRotatedZ.points[2] = matRotZ * f.points[2];

            // Rotate in X-Axis
            faceRotatedZX.points[0] = matRotX * faceRotatedZ.points[0];
            faceRotatedZX.points[1] = matRotX * faceRotatedZ.points[1];
            faceRotatedZX.points[2] = matRotX * faceRotatedZ.points[2];

            // Translate face away from camera
            int entityId = meshes.MirrorIdToEntityId(i);
            Face faceTranslated = faceRotatedZX;
            faceTranslated.points[0] = faceTranslated.points[0] + transforms.Get(entityId)->v;
            faceTranslated.points[1] = faceTranslated.points[1] + transforms.Get(entityId)->v;
            faceTranslated.points[2] = faceTranslated.points[2] + transforms.Get(entityId)->v;

            // Calculate face normal
            Vec3 line1 = faceTranslated.points[1] - faceTranslated.points[0];
            Vec3 line2 = faceTranslated.points[2] - faceTranslated.points[0];
            Vec3 normal = line1.CrossProduct(line2);
            normal.Normalize();

            // Normal projection onto line from camera to face
            float faceNormalProjectionOntoCameraRay =  normal * (faceTranslated.points[0] - camera);

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
                faceProjected.points[0] = g_projectionMatrix*faceTranslated.points[0];
                faceProjected.points[1] = g_projectionMatrix*faceTranslated.points[1];
                faceProjected.points[2] = g_projectionMatrix*faceTranslated.points[2];

                // Scale from normalized to screen size
                faceProjected.points[0].x = (faceProjected.points[0].x + 1) * 0.5 * (float) SCREEN_WIDTH;
                faceProjected.points[1].x = (faceProjected.points[1].x + 1) * 0.5 * (float) SCREEN_WIDTH;
                faceProjected.points[2].x = (faceProjected.points[2].x + 1) * 0.5 * (float) SCREEN_WIDTH;
                faceProjected.points[0].y = (faceProjected.points[0].y + 1) * 0.5 * (float) SCREEN_HEIGHT;
                faceProjected.points[1].y = (faceProjected.points[1].y + 1) * 0.5 * (float) SCREEN_HEIGHT;
                faceProjected.points[2].y = (faceProjected.points[2].y + 1) * 0.5 * (float) SCREEN_HEIGHT;

                faceProjected.colour.r *= dp;
                faceProjected.colour.g *= dp;
                faceProjected.colour.b *= dp;

                facesToRender.push_back(faceProjected);
            }
        }
    }

    std::sort(facesToRender.begin(), facesToRender.end());
    for(auto f : facesToRender)
    {
        drawFilledTriangle(f, f.colour);
    }
}