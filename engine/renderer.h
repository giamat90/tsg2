#pragma once

#include "tsg2.h"
#include <exception>
//#include "window.h"
#include "geometry.h"
#include "sprite.h"
#include "texture.h"
#include "color.h"
#include "font.h"
#include "asset.h"
#include "game_object.h"	// drawable
#include <vector>

template <typename WindowImpl, typename RendererImpl>
class renderer {
public:
	class creation_exception : public std::exception {
		const char* what() { return "renderer creation exception"; }
	};
	renderer(WindowImpl* w) : m_window(w) {};
	virtual ~renderer() = default;
public: // pure-virtual methods
	void render() { static_cast<RendererImpl*>(this)->render(); };
	void clear() { static_cast<RendererImpl*>(this)->clear(); };
	void set_draw_color(const color& c) { static_cast<RendererImpl*>(this)->set_draw_color(c); };
	void draw(sprite* s) { static_cast<RendererImpl*>(this)->draw(s); };
	void draw(texture* t) { static_cast<RendererImpl*>(this)->draw(t); };
	void draw(font* f) { static_cast<RendererImpl*>(this)->draw(f); };
	void draw(geometry::shape* s) { static_cast<RendererImpl*>(this)->draw(s); };
	void draw(const geometry::box3D& b) { static_cast<RendererImpl*>(this)->draw(b); };
	void draw(const geometry::box2D& b) { static_cast<RendererImpl*>(this)->draw(b); };
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_drawable(drawable* d){
		m_drawables.push_back(d);
	}
protected:
	WindowImpl* m_window;
	std::vector<drawable*> m_drawables;
};
