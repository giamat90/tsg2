#include "glfw_font.h"
#include "glfw_converter.h"

font* font::create_font() {
	return new glfw_font();
}

glfw_font::glfw_font() : font() {}

glfw_font::glfw_font(const std::string& file_name, const color& c, const text_position& p, const std::size_t dim) : font(file_name, c, p) {
	load_font(file_name, dim);
}

glfw_font::~glfw_font() {
}

void glfw_font::load_font(const std::string& file_name, const std::size_t dim) {
	//m_file_name = file_name;
	//m_glfw_component = TTF_OpenFont(file_name.c_str(), static_cast<float>(dim));
	//if (nullptr == m_glfw_component) {
	//	tsg::print("error opening font {}", glfw_GetError());
	//}
}