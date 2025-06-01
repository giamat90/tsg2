#include "arrow.h"
#include <texture.h>

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
	m_texture->set_size(25, 25);
	//m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow.png");
	//m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow_cut.jpg");
	m_texture->load("C:\\tsg2\\test\\arrow_bubble\\assets\\arrow.jpg");
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
}

void arrow::update(const float delta_time) {
	/* ToDo */
	physical_object::update(delta_time);
	m_texture->set_where(texture::position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]));
}

void arrow::process_input(input* input) {
	if (input->is_key_pressed(input::INPUT_KEY::KEY_W)) {
		tsg::print("Key w pressed!!! :D");
		m_linear_speed = 10.0f;

		/* Go up */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_A)) {
		/* Go left */
	}
	else if (input->is_key_pressed(input::INPUT_KEY::KEY_S)) {
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
	}

}