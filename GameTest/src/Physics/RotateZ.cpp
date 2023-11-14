#include "stdafx.h"
/*

// Rotate in Z-Axis
faceRotatedZ.points[0] = matRotZ * f.points[0];
faceRotatedZ.points[1] = matRotZ * f.points[1];
faceRotatedZ.points[2] = matRotZ * f.points[2];

*/

/*
    // Set up rotation matrices
    theta += 0.001f * deltaTime;

    // Rotation Z
    matRotZ(0,0) = cosf(theta);
    matRotZ(0,1) = sinf(theta);
    matRotZ(1,0) = -sinf(theta);
    matRotZ(1,1) = cosf(theta);
    matRotZ(2,2) = 1;
    matRotZ(3,3) = 1;

    // Rotation X
    matRotX(0,0) = 1;
    matRotX(1,1) = cosf(theta * 0.5f);
    matRotX(1,2) = sinf(theta * 0.5f);
    matRotX(2,1) = -sinf(theta * 0.5f);
    matRotX(2,2) = cosf(theta * 0.5f);
    matRotX(3,3) = 1;
    
    */