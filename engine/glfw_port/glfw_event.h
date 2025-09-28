#pragma once

#if GLFW_GAME

#include "tsg2.h"
#include "game_event.h"
#include "glfw_window.h"

class TSG2_API glfw_event : public game_event<glfw_window, glfw_event>{
public:
	GAME_EVENTS get_events();
};

#endif