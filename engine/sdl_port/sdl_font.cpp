#include "sdl_font.h"
#include "sdl_converter.h"

font* font::create_font() {
	return new sdl_font();
}

sdl_font::sdl_font() : font() {}

sdl_font::sdl_font(const std::string& file_name, const color& c, const text_position& p, const std::size_t dim) : font(file_name, c, p) {
	load_font(file_name, dim);
}

sdl_font::~sdl_font() {
	if (m_sdl_component) {
		TTF_CloseFont(m_sdl_component);
	}
	else {
		TTF_CloseFont(NULL);
	}
}

void sdl_font::load_font(const std::string& file_name, const std::size_t dim) {
	m_file_name = file_name;
	m_sdl_component = TTF_OpenFont(file_name.c_str(), static_cast<float>(dim));
	if (nullptr == m_sdl_component) {
		tsg::print("error opening font {}", SDL_GetError());
	}
}