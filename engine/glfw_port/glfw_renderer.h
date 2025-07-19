#pragma once

#include "../tsg2.h"
#include "../renderer.h"
#include "glfw_window.h"
#include "shader.h"
#include "vertex.h"

class TSG2_API glfw_renderer : public renderer<glfw_renderer>
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
	glfw_renderer(glfw_window * w);
	~glfw_renderer();
public: // overrided methods
	void render();
	void clear();
	void set_draw_color(const color&);
	void draw(sprite*);
	void draw(texture*);
	void draw(font*);
	void draw(geometry::shape*);
	void draw(const geometry::box3D&);
private:
	shader m_shader;
	sprite_vertex m_vertex;
};

