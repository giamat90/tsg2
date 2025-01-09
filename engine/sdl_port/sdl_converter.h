#pragma once

#include "../tsg2.h"
#include "geometry.h"
#include "color.h"
#include <SDL3/SDL.h>

class TSG2_API sdl_converter {
public:
	static inline SDL_FRect convert(const geometry::rectangle& r) {
		SDL_FRect ret{ .x = r.get_x(), .y = r.get_y(), .w = r.get_w(), .h = r.get_h() };
		return ret;
	}
	static inline geometry::rectangle convert(const SDL_FRect& r) {
		geometry::rectangle ret(r.x, r.y, r.w, r.h);
		return ret;
	}
	static inline SDL_Surface* convert(const surface& s) {
		/*
		SDL_Surface ret{ 
			.w = static_cast<uint8_t>(s[geometry::MEASURE::WIDTH]),
			.h = static_cast<uint8_t>(s[geometry::MEASURE::HEIGHT])
		};
		return ret;
		*/
		return SDL_CreateSurface(
			static_cast<uint8_t>(s[geometry::MEASURE::WIDTH]),
			static_cast<uint8_t>(s[geometry::MEASURE::HEIGHT]),
			SDL_PIXELFORMAT_UNKNOWN);
	}
	static inline SDL_Color convert(const color& c) {
		SDL_Color ret {
			.r = static_cast<uint8_t>(c[color::RGBA::RED]),
			.g = static_cast<uint8_t>(c[color::RGBA::GREEN]),
			.b = static_cast<uint8_t>(c[color::RGBA::BLUE]),
			.a = static_cast<uint8_t>(c[color::RGBA::ALPHA])
		};
		return ret;
	}
	static inline color convert(const SDL_Color& c) {
		color ret(c.r, c.g, c.b, c.a);
		return ret;
	}
};