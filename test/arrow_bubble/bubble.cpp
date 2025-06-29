#include "bubble.h"
#include <texture.h>
#include <tsg/os.h> // get_exe_path
#include <random>

bubble::bubble() {
	m_texture = texture::create_texture();
}

void bubble::init() {
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.png")).string());
	m_texture->set_scale(0.5f);
	auto w = m_texture->get_size().get_x();
	auto h = m_texture->get_size().get_y();
	set_box(m_position - geometry::point3D(w / scalar(2), h / scalar(2), 0.0f), m_position + geometry::point3D(w / scalar(2), h / scalar(2), 0.0f));
	// set random position	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<scalar> dis(-1.0, 1.0);
	m_box.translate({ dis(gen), dis(gen), scalar(0)}); // where it should start
	tsg::print("Bubble start at ({},{},{})",
		m_box.get_center().get<geometry::AXES::X>(),
		m_box.get_center().get<geometry::AXES::Y>(),
		m_box.get_center().get<geometry::AXES::Z>());
	set_mass(scalar(1));
	m_velocity = { scalar(0.75), scalar(1), scalar(0) };
}

void bubble::update(const scalar delta_time) {
	/* ToDo */
	//m_velocity = { scalar(1), scalar(1), scalar(0) };
	//m_rotation = scalar(1) / delta_time;
	//
	physical_object::update(delta_time);
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y], m_position[geometry::AXES::Z]));
	m_texture->set_rotation(m_rotation);
}