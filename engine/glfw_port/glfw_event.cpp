#if USE_GLFW
#include "glfw_event.h"
#include "glfw_window.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "gl_utility.h"

glfw_event::GAME_EVENTS glfw_event::get_events() {
	glfwPollEvents();
	if (glfwWindowShouldClose(m_window.get_adaptee())) {
		//glfwSetWindowShouldClose(w->get_adaptee(), GL_TRUE);
		return glfw_event::GAME_EVENTS::QUIT;
	}
	gl_check_error(__FILE__, __LINE__);
}

#endif