#include "arrow.h"
#include <texture.h>
#include <tsg/os.h>
#include <cmath>

using geometry::scalar;

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow2.png")).string());
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
	auto w = m_texture->get_size().get_x();
	auto h = m_texture->get_size().get_y();
	set_box(m_position - geometry::point3D(w / scalar(2), h / scalar(2), 0.0f), m_position + geometry::point3D(w / scalar(2), h / scalar(2), 0.0f));
}

void arrow::update(const float delta_time) {
	/* ToDo */
	physical_object::update(delta_time);
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
	m_texture->set_rotation(m_rotation);
}

void arrow::process_input(input* input) {
	m_velocity.zero();
	if (input->is_key_pressed(input::INPUT_KEY::KEY_W)) {
		/* Go up */
		m_velocity += geometry::point3D(0.0, 1.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_A)) {
		/* Go left */
		m_velocity += geometry::point3D(-1.0, 0.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_S)) {
		/* Go down */
		m_velocity += geometry::point3D(0.0, -1.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_D)) {
		/* Go right */
		m_velocity += geometry::point3D(1.0, 0.0, 0.0);
	}

	constexpr geometry::scalar rotation_unit{ 0.05f };
	if (input->is_key_pressed(input::INPUT_KEY::KEY_LEFT)) {
		/* Rotate left */
		m_rotation += rotation_unit;
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_RIGHT)) {
		/* Rotate right */
		m_rotation += -rotation_unit;
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_UP)) {
		/* Rotate right */
		m_velocity += geometry::point3D(std::cos(m_rotation), std::sin(m_rotation), 0.0f);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_DOWN)) {
		/* Rotate right */
		m_velocity -= geometry::point3D(std::cos(m_rotation), std::sin(m_rotation), 0.0f);
	}

	if (input->is_key_pressed(input::INPUT_KEY::KEY_SPACE)) {
		/* JUMP */
	}
}