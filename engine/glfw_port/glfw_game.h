#pragma once

#ifdef GLFW_GAME

#include "tsg2.h"

class TSG2_API glfw_game {
public:
	glfw_game();
	~glfw_game();
protected:
	bool init();
	void quit();
};
#endif