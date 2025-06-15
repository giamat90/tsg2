#include "bubble.h"
#include <texture.h>
#include <tsg/os.h> // get_exe_path

bubble::bubble() {
	m_texture = texture::create_texture();
}

void bubble::init() {
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.png")).string());
	m_texture->set_where(texture::position(-0.75f, -0.75f));
	auto w = m_texture->get_size().get_x();
	auto h = m_texture->get_size().get_y();
	set_box(m_position - geometry::point3D(w / scalar(2), h / scalar(2), 0.0f), m_position + geometry::point3D(w / scalar(2), h / scalar(2), 0.0f));
}

void bubble::update(const float delta_time) {
	/* ToDo */
	m_velocity = { scalar(1), scalar(1), scalar(0) };
	m_rotation = scalar(1) / delta_time;
	//
	physical_object::update(delta_time);
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
	m_texture->set_rotation(m_rotation);
}