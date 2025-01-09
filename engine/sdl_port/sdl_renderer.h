#pragma once

#include "../tsg2.h"
#include "renderer.h"
#include "sdl_adaptor.h"
#include <SDL3/SDL_render.h>
#include "sdl_window.h"

class TSG2_API sdl_renderer : public renderer, public sdl_adaptor<SDL_Renderer>
{
public:
	template <typename S>
	class exception : public std::exception {
	public:
		exception(S s) : std::exception(), m_s(s) {};
		S what() { return m_s; }
	private:
		S m_s{};
	};
public:
	sdl_renderer(sdl_window * w);
	~sdl_renderer();
public: // overrided methods
	void render() override final;
	void clear() override final;
	void set_draw_color(const color&) override final;
	void draw(sprite*) override final;
	void draw(texture*) override final;
	void draw(font*) override final;
	void draw(geometry::rectangle&) override final;
};

