#pragma once

#if USE_GLFW

#include "game_event.h"
#include "glfw_window.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

game_event::GAME_EVENTS game_event::get_events(game* g) {
	if (g) {		
		glfwPollEvents();
		if (auto w = dynamic_cast<glfw_window*>(g->get_window())) {
			if (glfwWindowShouldClose(w->get_adaptee())) {
				return GAME_EVENTS::QUIT;
			}
		}
		tsg::print(glGetError());

	}
	else {
		throw;
	}
}

#endif