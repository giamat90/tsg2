#include "arrow.h"
#include <texture.h>
#include <tsg/os.h>

#define ARROW_TEXTURE

using geometry::scalar;
using geometry::cos;

arrow::arrow() {
	m_texture = texture::create_texture();
}

void arrow::init() {
#ifdef ARROW_TEXTURE
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow2.png")).string());
#else
	m_texture->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\astronaut_sprite_3.png")).string());
#endif
	m_texture->set_where(texture::position({ m_position[geometry::AXES::X], m_position[geometry::AXES::Y], m_position[geometry::AXES::Z] }));
	m_texture->set_scale(0.5f);
	auto w = m_texture->get_size().get<geometry::AXES::X>();
	auto h = m_texture->get_size().get<geometry::AXES::Y>();
	set_box(m_position, { w / scalar(2), h / scalar(2), 0.0f });
	set_mass(scalar(1));
}

void arrow::update(const scalar delta_time) {
	/* ToDo */
	physical_object::update(delta_time);
	m_texture->set_where(texture::position({ m_position[geometry::AXES::X], m_position[geometry::AXES::Y], m_position[geometry::AXES::Z] }));
	m_texture->set_rotation(m_rotation);
}

void arrow::process_input(input* input) {
	//m_velocity.zero();
	if (input->is_key_pressed(input::INPUT_KEY::KEY_W)) {
		/* Go up */
		push({ scalar(0), scalar(1), scalar(0) });
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_A)) {
		/* Go left */
		push({ scalar(-1), scalar(0), scalar(0) });
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_S)) {
		/* Go down */
		push({ scalar(0), scalar(-1), scalar(0) });
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_D)) {
		/* Go right */
		push({ scalar(1), scalar(0), scalar(0) });
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
		push({ geometry::cos(m_rotation), geometry::sin(m_rotation), 0.0f });
	}
	if (input->is_key_pressed(input::INPUT_KEY::KEY_DOWN)) {
		/* Rotate right */
		push({ -geometry::cos(m_rotation), -geometry::sin(m_rotation), 0.0f });
	}

	if (input->is_key_pressed(input::INPUT_KEY::KEY_SPACE)) {
		/* JUMP */
	}
}