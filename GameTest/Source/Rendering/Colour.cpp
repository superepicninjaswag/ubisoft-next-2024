#include "stdafx.h"

#include "Colour.h"

Colour::Colour()
{
    r = 1.0f;
    g = 1.0f;
    b = 1.0f;
}

Colour::Colour(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}