#pragma once

#include "../Rendering/Colour.h"

/*
* The cake was a lie and so is this component. There is not actual texture information beyond what colours the triangles should be.
*/
struct TextureComponent
{
    Colour outline;
    Colour fill;

    TextureComponent(Colour o);
    TextureComponent(Colour o, Colour f);

    bool isFilled();
};
