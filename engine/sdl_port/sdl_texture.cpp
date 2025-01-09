#include "sdl_texture.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "io/io.h"
#include "sdl_converter.h"

texture* texture::create_texture() {
	return new sdl_texture();
}

sdl_texture::sdl_texture(SDL_Texture* sdl_t) {
	m_sdl_component = sdl_t ? sdl_t : nullptr;
}

sdl_texture::~sdl_texture() {
	SDL_DestroyTexture(m_sdl_component);
	m_sdl_component = nullptr;
}
