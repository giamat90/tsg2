#pragma once

#include <tsg2.h>
#include <tsg/types.h>
#include <window.h>

class TSG2_API glfw_window : public window, public tsg::adapter<GLFWwindow> {
public:
	glfw_window(const std::string& s, window::width_t w, window::height_t h);
	~glfw_window();
};

