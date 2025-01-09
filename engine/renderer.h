#pragma once

#include "tsg2.h"
#include <exception>
#include "window.h"
#include "geometry.h"
#include "sprite.h"
#include "texture.h"
#include "color.h"
#include "font.h"
#include "asset.h"

class TSG2_API renderer {
public:
	class creation_exception : public std::exception {
		const char* what() { return "renderer creation exception"; }
	};
	renderer(window * w) : m_window(w) {};
	virtual ~renderer() = default;
public: // pure-virtual methods
	virtual void render() = 0;
	virtual void clear() = 0;
	virtual void set_draw_color(const color&) = 0;
	virtual void draw(sprite*) = 0;
	virtual void draw(texture*) = 0;
	virtual void draw(font*) = 0;
	virtual void draw(geometry::rectangle&) = 0;
protected:
	window* m_window{ nullptr };
};
