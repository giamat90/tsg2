#pragma once

#include "tsg2.h"
#include "renderer.h"
#include "glfw_window.h"
#include "shader.h"
#include "vertex.h"

class TSG2_API glfw_renderer : public renderer<glfw_window, glfw_renderer>
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
	void draw(texture*);
	void draw(sprite*);
	void draw(mesh*);
	void draw(font*);
	void draw(geometry::bounding_volume*);
	void draw(const geometry::box3D&);
	void draw(const geometry::box2D&); 
	void draw(const drawable_bounding_volume&);
private:
	/* to render 2D texture */
	shader m_texture_shader;
	texture_vertex m_texture_vertex;
	/* to render 2D sprites */
	shader m_sprite_shader;
	texture_vertex m_sprite_vertex;
	/* to render 3D meshes */ 
	shader m_mesh_shader;
	mesh_vertex m_mesh_vertex;
	/* for bounding volumes */
	shader m_line_shader;
	box2D_vertex m_box2D_vertex;
};

