#pragma once

#ifdef USE_GLFW

#include "../game.h"

class glfw_game {
public:
	bool init();
	void quit();
};
#endif