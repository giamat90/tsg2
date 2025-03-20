#include "glfw_window.h"
#include <tsg/types.h>
#include <tsg/io.h>
#include <glad/gl.h>

glfw_window::glfw_window(const std::string& t, const width_t w, const height_t h) : window(t, w, h) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create an GLFW Window
	m_adaptee = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);

	if (!m_adaptee)
	{
		throw create_exception();
	}
	glfwMakeContextCurrent(m_adaptee);
}

glfw_window::~glfw_window() {
	// in openGL it is handled with context that are free in glfwTerminate
	// or maybe I should?
	glfwDestroyWindow(m_adaptee);
}


