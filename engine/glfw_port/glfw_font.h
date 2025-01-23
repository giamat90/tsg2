#pragma once

#include "../tsg2.h"
#include "font.h"
#include "glfw_adaptor.h"
#include <string>

class TSG2_API glfw_font : public font {
public:
	glfw_font();
	glfw_font(const std::string& file_name, const color& c, const text_position& p, const std::size_t dim);
	~glfw_font();
	void load_font(const std::string& file_name, const std::size_t dim) override;
};