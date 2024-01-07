#pragma once

#include "../Rendering/Colour.h"

struct TextureComponent
{
    Colour outline;
    Colour fill;

    TextureComponent(Colour o);
    TextureComponent(Colour o, Colour f);
};
