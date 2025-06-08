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
}

void arrow::process_input(input* input) {
	if (input->is_key_pressed(input::INPUT_KEY::KEY_W)) {
		m_linear_speed = 1.0;

		/* Go up */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_A)) {
		/* Go left */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_S)) {
		m_linear_speed = -1.0;
		/* Go down */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_D)) {
		/* Go right */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_SPACE)) {
		/* JUMP */
	}
	else {
		tsg::print("Key w not pressed!!! D:");
		m_linear_speed = 0.0f;
	}

}