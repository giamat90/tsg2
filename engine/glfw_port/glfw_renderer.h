#pragma once

#include "../tsg2.h"
#include "renderer.h"
#include "glfw_window.h"
#include "shader.h"
#include "vertex.h"

class TSG2_API glfw_renderer : public renderer
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
	void render() override final;
	void clear() override final;
	void set_draw_color(const color&) override final;
	void draw(sprite*) override final;
	void draw(texture*) override final;
	void draw(font*) override final;
	void draw(geometry::rectangle&) override final;
private:
	shader m_shader;
	sprite_vertex m_vertex;
};

