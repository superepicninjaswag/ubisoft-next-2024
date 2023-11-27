#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() {}

char textBuffer[64];

void Renderer::Init()
{
    facesToRender.reserve(defaultBufferSize);
    lightDirection.Normalize();
    setProjectionMatrix();

    // Define clipping planes
    // Bottom
    clippingPlanePoints.emplace_back(0.0f, -1.0f, 0.0f);
    clippingPlaneNormals.emplace_back(0.0f, 1.0f, 0.0f);

    // Top
    clippingPlanePoints.emplace_back( 0.0f, 1.0f, 0.0f);
    clippingPlaneNormals.emplace_back(0.0f, -1.0f, 0.0f);

    // Left
    clippingPlanePoints.emplace_back( -1.0f, 0.0f, 0.0f);
    clippingPlaneNormals.emplace_back(1.0f, 0.0f, 0.0f);

    // Right
    clippingPlanePoints.emplace_back(1.0f, 0.0f, 0.0f);
    clippingPlaneNormals.emplace_back(-1.0f, 0.0f, 0.0f);

    // Near
    clippingPlanePoints.emplace_back( 0.0f, 0.0f, 1.0f);
    clippingPlaneNormals.emplace_back(0.0f, 0.0f, -1.0f);
}

void Renderer::Render(Pool<MeshComponent>& meshes, Pool<TransformComponent>& transforms, Pool<MeshResourceComponent>& meshResources)
{
    // Camera matrices
    setCameraMatrices();
    cameraAndProjectionMatrix = inverseCameraMatrix * projectionMatrix;

    // Object-order render loop
    for (int i = 0; i < meshes.Size(); i++)
    {
        int meshResourceId = meshes._dense[i].meshResourceId;
        int entityId = meshes.MirrorIdToEntityId(i);

        Matrix4x4 rX, rY, rZ, translationMatrix;

        rX.rotationX(theta);
        rY.rotationY(theta * 0.5f);
        rZ.rotationZ(theta * 0.25f);

        Vec3 position = transforms.Get(entityId)->v;
        translationMatrix.translation(position.x, position.y, position.z);

        worldMatrix = rX * rY * rZ * translationMatrix;

        for (auto& f : meshResources.Get(meshResourceId)->faces)
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
            float faceNormalProjectionOntoCameraRay = normal * (faceTransformed.points[0] - camera);

            /*
            If the projection of the face normal is negative (relative
            to a ray from the camera), then we know the face is pointing
            away from the camera
            */
            if (faceNormalProjectionOntoCameraRay < 0.0f)
            {
                // Project the face from world space to camera space to normalized
                Face faceProjected;
                for (int i = 0; i < 3; i++)
                {
                    faceProjected.points[i] = cameraAndProjectionMatrix * faceTransformed.points[i];
                    faceProjected.points[i] = faceProjected.points[i] * (1.0f / faceProjected.points[i].w);
                    std::cout << faceProjected.points[i].z << "\n";
                }

                // Clip triangle
                std::list<Face> clippedTriangles;
                clippedTriangles.push_back(faceProjected);
                for (int i = 0; i < clippingPlanePoints.size(); i++)
                {
                    int numTriangles = clippedTriangles.size();
                    int j = 0;
                    while (j < numTriangles)
                    {
                        Face f = clippedTriangles.front();
                        clippedTriangles.pop_front();
                        std::vector<Face> newClippedTriangles = f.clipAgainstPlane(clippingPlanePoints[i], clippingPlaneNormals[i]);
                        clippedTriangles.insert(clippedTriangles.end(), newClippedTriangles.begin(), newClippedTriangles.end());
                        j++;
                    }
                }

                for (int i = 0; i < clippedTriangles.size(); i++)
                {
                    Face f = clippedTriangles.front();
                    clippedTriangles.pop_front();
                    // Scale from normalized to screen size
                    for (int j = 0; j < 3; j++)
                    {
                        f.points[j].x = (faceProjected.points[j].x + 1) * 0.5 * SCREEN_WIDTH;
                        f.points[j].y = (faceProjected.points[j].y + 1) * 0.5 * SCREEN_HEIGHT;
                    }

                    // Lighting
                    //float dp = normal * lightDirection;
                    //f.colour.r = dp / 2 + faceProjected.colour.r / 2;
                    //f.colour.g = dp / 2 + faceProjected.colour.g / 2;
                    //f.colour.b = dp / 2 + faceProjected.colour.b / 2;

                    // Add face to render queue
                    facesToRender.push_back(f);
                }
            }
        }
    }

    // Painter's algorithm
    std::sort(facesToRender.begin(), facesToRender.end());
    for (auto& f : facesToRender)
    {
        drawFilledTriangle(f, f.colour);
        f.colour.r = 0.0f;
        f.colour.g = 0.0f;
        f.colour.b = 0.0f;
        drawTriangle(f, f.colour);
    }

    // I don't want faces from previous frames to linger in the buffer
    facesToRender.clear();
}

void Renderer::setProjectionMatrix()
{
    // Matrix definition taken from Real-time rendering textbook
    float pi = 3.14159265;
    float fovRad = 1.0f / tanf(fovDeg / 2.0f / 180.0f * pi);
    projectionMatrix(0, 0) = fovRad / aspectRatio;
    projectionMatrix(1, 1) = fovRad;
    projectionMatrix(2, 2) = (zFar + zNear) / (zFar - zNear);
    projectionMatrix(2, 3) = 1.0f;
    projectionMatrix(3, 2) = -(zFar * zNear) / (zFar - zNear);
}

void Renderer::setCameraMatrices()
{
    Vec3 cameraTarget = Vec3(0.0f, 0.0f, 1.0f);
    Matrix4x4 cameraRotation;
    cameraRotation.rotationY(yaw);
    cameraLookDirection = cameraRotation * cameraTarget;
    cameraTarget = camera + cameraLookDirection;

    Vec3 forward = cameraTarget - camera;
    forward.Normalize();

    up = up - (forward * (up * forward));
    up.Normalize();

    right = up.CrossProduct(forward);

    cameraMatrix(0, 0) = right.x;    cameraMatrix(0, 1) = right.y;    cameraMatrix(0, 2) = right.z;    cameraMatrix(0, 3) = 0.0f;
    cameraMatrix(1, 0) = up.x;       cameraMatrix(1, 1) = up.y;       cameraMatrix(1, 2) = up.z;       cameraMatrix(1, 3) = 0.0f;
    cameraMatrix(2, 0) = forward.x;  cameraMatrix(2, 1) = forward.y;  cameraMatrix(2, 2) = forward.z;  cameraMatrix(2, 3) = 0.0f;
    cameraMatrix(3, 0) = camera.x;   cameraMatrix(3, 1) = camera.y;   cameraMatrix(3, 2) = camera.z;   cameraMatrix(3, 3) = 1.0f;

    inverseCameraMatrix(0, 0) = cameraMatrix(0, 0); inverseCameraMatrix(0, 1) = cameraMatrix(1, 0); inverseCameraMatrix(0, 2) = cameraMatrix(2, 0); inverseCameraMatrix(0, 3) = 0.0f;
    inverseCameraMatrix(1, 0) = cameraMatrix(0, 1); inverseCameraMatrix(1, 1) = cameraMatrix(1, 1); inverseCameraMatrix(1, 2) = cameraMatrix(2, 1); inverseCameraMatrix(1, 3) = 0.0f;
    inverseCameraMatrix(2, 0) = cameraMatrix(0, 2); inverseCameraMatrix(2, 1) = cameraMatrix(1, 2); inverseCameraMatrix(2, 2) = cameraMatrix(2, 2); inverseCameraMatrix(2, 3) = 0.0f;
    inverseCameraMatrix(3, 0) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 0) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 0) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 0));
    inverseCameraMatrix(3, 1) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 1) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 1) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 1));
    inverseCameraMatrix(3, 2) = -(cameraMatrix(3, 0) * inverseCameraMatrix(0, 2) + cameraMatrix(3, 1) * inverseCameraMatrix(1, 2) + cameraMatrix(3, 2) * inverseCameraMatrix(2, 2));
    inverseCameraMatrix(3, 3) = 1.0f;
}
