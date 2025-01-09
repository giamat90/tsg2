#include "sdl_renderer.h"
#include "sdl_converter.h"
#include "sdl_texture.h"
#include "sdl_font.h"
#include "io/io.h"

sdl_renderer::sdl_renderer(sdl_window * w) : renderer(w) {
	//// Create SDL renderer
	m_sdl_component = SDL_CreateRenderer(w->m_sdl_window, NULL);

	if (!m_sdl_component)
	{
		throw creation_exception();
	}
}

sdl_renderer::~sdl_renderer() {
	SDL_DestroyRenderer(m_sdl_component);
}

// overrided methods

void sdl_renderer::render() {
	SDL_RenderPresent(m_sdl_component);
}

void sdl_renderer::clear() {
	SDL_RenderClear(m_sdl_component);
}

void sdl_renderer::set_draw_color(const color& c)
{
	if (!SDL_SetRenderDrawColor(m_sdl_component, c[color::RGBA::RED], c[color::RGBA::GREEN], c[color::RGBA::BLUE], c[color::RGBA::ALPHA])) {
		throw std::exception();
	}
}

void sdl_renderer::draw(sprite* s) {

	// Load from file
	//SDL_Surface* surf = IMG_Load(s->get_file().c_str());
}

void sdl_renderer::draw(texture* t) {
	auto tmp_texture = dynamic_cast<sdl_texture*>(t);
	SDL_FRect dest{
		.x = tmp_texture->get_where().get_x(),
		.y = tmp_texture->get_where().get_y(),
		.w = static_cast<float>(tmp_texture->get_sdl_component()->w),
		.h = static_cast<float>(tmp_texture->get_sdl_component()->h)
	};
	SDL_RenderTexture(m_sdl_component, tmp_texture->get_sdl_component(), NULL, &dest);
}

void sdl_renderer::draw(font* f) {
	auto tmp_font = dynamic_cast<sdl_font*>(f);
	if (nullptr == tmp_font) {
		throw(exception("Error converting font in sdl_font"));
	}
	SDL_Surface* tmp_surface = TTF_RenderText_Solid(
		tmp_font->get_sdl_component(), 
		tmp_font->get_text().c_str(), 
		tmp_font->get_text().size(), 
		sdl_converter::convert(tmp_font->get_color()));

	SDL_Texture* tmp_texture = SDL_CreateTextureFromSurface(m_sdl_component, tmp_surface);
	SDL_FRect dest{
		.x = tmp_font->get_where().get_x(),
		.y = tmp_font->get_where().get_y(),
		.w = static_cast<float>(tmp_surface->w),
		.h = static_cast<float>(tmp_surface->h)
	};
	SDL_RenderTexture(m_sdl_component, tmp_texture, NULL, &dest);

	SDL_DestroyTexture(tmp_texture);
	SDL_DestroySurface(tmp_surface);
}

void sdl_renderer::draw(geometry::rectangle& r) {
	SDL_FRect tmp{ r.get_x(), r.get_y(), r.get_w(), r.get_h() };
	if (!SDL_RenderFillRect(m_sdl_component, &tmp)) {
		throw std::exception();
	}
}