#include "arrow.h"
#include <texture.h>

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
	m_texture->set_size(50, 50);
	m_texture->load("C:\\tsg2\\test\\openGL_tests\\assets\\arrow.png");
	//m_texture->load("C:\\Workspace\\GiaMat90\\tsg2\\test\\openGL_tests\\assets\\arrow.png");
}

void arrow::update(const float delta_time) {
	/* ToDo */
}