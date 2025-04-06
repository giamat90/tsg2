#include "arrow.h"
#include <texture.h>

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
	m_texture->set_size(50, 50);
	//m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow.png");
	//m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow_cut.jpg");
	m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow.jpg");
	m_texture->set_where(texture::position(0.0f, 0.0f));
}

void arrow::update(const float delta_time) {
	/* ToDo */
}