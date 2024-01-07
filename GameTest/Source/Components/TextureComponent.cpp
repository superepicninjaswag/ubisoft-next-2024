#include "stdafx.h"

#include "TextureComponent.h"


TextureComponent::TextureComponent(Colour o)
{
	outline.r = o.r;
	outline.g = o.g;
	outline.b = o.b;
	fill.r = -1.0f;
	fill.g = -1.0f;
	fill.b = -1.0f;
}

TextureComponent::TextureComponent(Colour o, Colour f)
{
	outline.r = o.r;
	outline.g = o.g;
	outline.b = o.b;
	fill.r = f.r;
	fill.g = f.g;
	fill.b = f.b;
}
