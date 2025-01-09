#pragma once

#include "../tsg2.h"
#include "sdl_adaptor.h"
#include "texture.h"
#include "asset.h"
#include <SDL3/SDL_render.h>

class TSG2_API sdl_texture : public texture, public sdl_adaptor<SDL_Texture>
{
public:
	sdl_texture() = default ;
	explicit sdl_texture(SDL_Texture*);
	virtual ~sdl_texture();
};

