#include <game.h>

#ifdef USE_SDL

#include "sdl_window.h"
#include "sdl_renderer.h"
#include "sdl_keyboard_input.h"
#include "sdl_game_timer.h"
#include "sdl_converter.h"
#include "sdl_texture.h"
#include "io/io.h"

#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>

bool game::initialize_externals() {
	// Initialize SDL
	bool res{ true };
	if (!(res = SDL_Init(SDL_INIT_VIDEO))) {
		tsg::print("Error initializing SDL");
	}
	if (!(res &= TTF_Init())) {
		tsg::print("Error initializing SDL_TTF");
	}

	return res;
}

void game::create_window(std::string title, window::width_t w, window::height_t h) {
	m_window = new sdl_window(title, w, h);
}

void game::create_renderer() {
	m_renderer = new sdl_renderer(static_cast<sdl_window*>(m_window));
}

void game::create_keyboard_input() {
	m_input = new sdl_keyboard_input();
}

void game::create_timer(const unsigned fps) {
	m_timer = new sdl_game_timer(fps);
}

texture* game::create_texture(const std::string& p) {
	SDL_Surface* tmp_surf = IMG_Load(p.c_str());
	auto tmp_renderer = dynamic_cast<sdl_renderer*>(m_renderer);
	if (tmp_surf && tmp_renderer) {
		auto tmp_texture = SDL_CreateTextureFromSurface(tmp_renderer->get_sdl_component(), tmp_surf);
		SDL_DestroySurface(tmp_surf);
		auto ret = new sdl_texture(tmp_texture);
		return ret;
	}
	else {
		tsg::print("creating texture error");
		return nullptr;
	}
}

texture* game::create_texture(surface* s) {
	auto tmp_surf = sdl_converter::convert(*s);
	auto tmp_renderer = dynamic_cast<sdl_renderer*>(m_renderer);
	if (tmp_surf && tmp_renderer) {
		auto tmp_texture = SDL_CreateTextureFromSurface(tmp_renderer->get_sdl_component(), tmp_surf);
		SDL_DestroySurface(tmp_surf);
		auto ret = new sdl_texture(tmp_texture);
		return ret;
	}
	else {
		tsg::print("creating texture error");
		return nullptr;
	}
}

void game::quit() {
	SDL_Quit();
	TTF_Quit();
}
#endif