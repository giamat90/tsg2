#pragma once

#include <tsg2.h>
#include <tsg/types.h>
#include <window.h>
#include "gl_includes.h"

class TSG2_API glfw_window : public window, public tsg::adapter<GLFWwindow> {
public:
	glfw_window(const std::string& s = "", const window::width_t w = 0u, const window::height_t h = 0u);
	~glfw_window();
};

