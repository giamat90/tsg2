#include "bubble.h"
#include <texture.h>

bubble::bubble() {
	m_texture = texture::create_texture();
}

void bubble::init() {
	m_texture->set_size(50, 50);
	m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\bubble.jpg");
	m_texture->set_where(texture::position(-0.75f, -0.75f));
	//m_texture->set_where(texture::texture_position(0.0f, 0.0f));
}

void bubble::update(const float delta_time) {
	/* ToDo */
	float delta = delta_time;
	m_texture->set_where(texture::position(
		m_texture->get_where().get_x() + delta,
		m_texture->get_where().get_y() + delta));
	m_texture->set_rotation(m_texture->get_rotation() + (1.0f / delta_time));
	m_texture->set_scale(1.0f);
}