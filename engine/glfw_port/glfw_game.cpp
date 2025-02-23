#include <game.h>

#ifdef USE_GLFW

#include "glfw_window.h"
#include "glfw_renderer.h"
#include "glfw_keyboard_input.h"
#include "glfw_game_timer.h"
#include "glfw_converter.h"
#include "glfw_texture.h"
#include <tsg/io.h>

#include <GLFW/glfw3.h>


bool game::initialize_externals() {
	// Initialize SDL
	bool res{ true };
	/* Initialize the library */
	if (GLFW_FALSE == glfwInit())
	{
		res = false;
	}

	//gladLoadGL(glfwGetProcAddress);
	/*
	if (!(res = glfw_Init(glfw_INIT_VIDEO))) {
		tsg::print("Error initializing SDL");
	}
	if (!(res &= TTF_Init())) {
		tsg::print("Error initializing glfw_TTF");
	}
	*/
	return res;
}

void game::create_window(std::string title, window::width_t w, window::height_t h) {
	m_window = new glfw_window(title, w, h);
}

void game::create_renderer() {
	m_renderer = new glfw_renderer(static_cast<glfw_window*>(m_window));
}

void game::create_keyboard_input() {
	m_input = new glfw_keyboard_input();
}

void game::create_timer(const unsigned fps) {
	m_timer = new glfw_game_timer(fps);
}

texture* game::create_texture(const std::string& p) {
	/*
	glfw_Surface* tmp_surf = IMG_Load(p.c_str());
	auto tmp_renderer = dynamic_cast<glfw_renderer*>(m_renderer);
	if (tmp_surf && tmp_renderer) {
		auto tmp_texture = glfw_CreateTextureFromSurface(tmp_renderer->get_glfw_component(), tmp_surf);
		glfw_DestroySurface(tmp_surf);
		auto ret = new glfw_texture(tmp_texture);
		return ret;
	}
	else {
		tsg::print("creating texture error");
		return nullptr;
	}
	*/
	return nullptr;
}

texture* game::create_texture(surface* s) {
	/*
	auto tmp_surf = glfw_converter::convert(*s);
	auto tmp_renderer = dynamic_cast<glfw_renderer*>(m_renderer);
	if (tmp_surf && tmp_renderer) {
		auto tmp_texture = glfw_CreateTextureFromSurface(tmp_renderer->get_glfw_component(), tmp_surf);
		glfw_DestroySurface(tmp_surf);
		auto ret = new glfw_texture(tmp_texture);
		return ret;
	}
	else {
		tsg::print("creating texture error");
		return nullptr;
	}
	*/
	return nullptr;
}

void game::quit() {
	glfwTerminate();
}
#endif