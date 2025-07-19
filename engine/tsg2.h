// GPL (General Purpose Library) export
//

#pragma once

#ifdef _WIN32
#if USE_GLFW
#include <GLFW/glfw3.h>
#endif

#ifdef TSG2_EXPORT
#define TSG2_API __declspec(dllexport)
#else
#define TSG2_API  __declspec(dllimport)
#endif
#else	// case unix
#define TSG2_API __attribute__ ((visibility ("default")))
#endif

#ifdef USE_GLFW
/* forward declarations: used in game.h */
class glfw_game;
class glfw_renderer; 
class glfw_game_timer;
class glfw_input;
class glfw_window;
class glfw_event;
#endif