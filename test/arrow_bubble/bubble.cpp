#include "bubble.h"
#include <texture.h>
#include <tsg/os.h> // get_exe_path

bubble::bubble() {
	m_texture = texture::create_texture();
}

void bubble::init() {
	m_texture->set_size(50, 50);
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.jpg")).string());
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