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