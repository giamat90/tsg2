#include "sdl_window.h"
#include <SDL3/SDL.h>

sdl_window::sdl_window(const std::string& t, const width_t w, const height_t h) : window(t, w, h) {
	// Create an SDL Window
	m_sdl_window = SDL_CreateWindow(m_title.c_str(), m_width, m_height, 0);

	if (!m_sdl_window)
	{
		throw create_exception();
	}
}

sdl_window::~sdl_window() {
	SDL_DestroyWindow(m_sdl_window);	
}

