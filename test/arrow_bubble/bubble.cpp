#include "bubble.h"
#include <sprite.h>
#include <tsg/logger.h>
#include <random>

bubble::bubble() {
	m_sprite = sprite::create_sprite();
}

void bubble::init() {
	m_sprite->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.png")).string());
	m_sprite->set_scale(0.5f);
	auto w = m_sprite->get_size().get<geometry::AXES::X>();
	auto h = m_sprite->get_size().get<geometry::AXES::Y>();
	// set random position	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<scalar> dis(-1.0, 1.0);
	auto b = geometry::box2D({ dis(gen), dis(gen) }, { scalar(w) / scalar(2), scalar(h) / scalar(2) });
	set_bounding_volume(b);
	tsg::logger::get_instance().write("Bubble start at ({},{})",
		m_box.get_center().get<geometry::AXES::X>(),
		m_box.get_center().get<geometry::AXES::Y>());
	set_mass(scalar(1));
	m_velocity = { dis(gen), dis(gen) };
}

void bubble::update(const scalar delta_time) {
	physical_object::update(delta_time);
	m_sprite->set_where(texture::position({ m_position[geometry::AXES::X], m_position[geometry::AXES::Y]}));
	m_sprite->set_rotation(m_rotation);
}