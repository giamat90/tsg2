#pragma once

#if USE_GLFW

#include "game_event.h"
#include "glfw_window.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "gl_utility.h"

game_event::GAME_EVENTS game_event::get_events(game* g) {
	if (g) {		
		glfwPollEvents();
		if (auto w = dynamic_cast<glfw_window*>(g->get_window())) {
			if (glfwWindowShouldClose(w->get_adaptee())) {
				//glfwSetWindowShouldClose(w->get_adaptee(), GL_TRUE);
				return GAME_EVENTS::QUIT;
			}
		}
		gl_check_error(__FILE__, __LINE__);

	}
	else {
		throw;
	}
}

#endif