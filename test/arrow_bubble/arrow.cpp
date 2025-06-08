#include "arrow.h"
#include <texture.h>
#include <tsg/os.h>

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
	m_texture->set_size(25, 25);
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow.jpg")).string());
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
}

void arrow::update(const float delta_time) {
	/* ToDo */
	physical_object::update(delta_time);
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
	m_texture->set_rotation(m_rotation);
}

void arrow::process_input(input* input) {
	m_linear_speed.zero();
	if (input->is_key_pressed(input::INPUT_KEY::KEY_W)) {
		/* Go up */
		m_linear_speed += geometry::point3D(0.0, 1.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_A)) {
		/* Go left */
		m_linear_speed += geometry::point3D(-1.0, 0.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_S)) {
		/* Go down */
		m_linear_speed += geometry::point3D(0.0, -1.0, 0.0);
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_D)) {
		/* Go right */
		m_linear_speed += geometry::point3D(1.0, 0.0, 0.0);
	}

	if (input->is_key_pressed(input::INPUT_KEY::KEY_LEFT)) {
		/* Rotate left */
		m_rotation += 0.05f;
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_RIGHT)) {
		/* Rotate right */
		m_rotation += -0.05f;
	}

	if (input->is_key_pressed(input::INPUT_KEY::KEY_SPACE)) {
		/* JUMP */
	}
}