#ifdef USE_GLFW

#include "glfw_game.h"
#include "gl_includes.h"

glfw_game::glfw_game() {
	init();
}

glfw_game::~glfw_game() {
	quit();
}

bool glfw_game::init() {
	// Initialize GL and GLFW
	bool res{ true };
	/* Initialize the library */
	if (GLFW_FALSE == glfwInit())
	{
		res = false;
	}
	return res;
}

void glfw_game::quit() {
	glfwTerminate();
}
#endif