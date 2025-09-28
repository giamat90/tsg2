#pragma once

#include "tsg2.h"
#include "gl_includes.h"

/* tsg includes */
#include <tsg/types.h>
#include <window.h>

class TSG2_API glfw_window : public window, public tsg::adapter<GLFWwindow> {
public:
	glfw_window(const std::string& s = "", const window::width_t w = 0u, const window::height_t h = 0u);
	~glfw_window();
};

