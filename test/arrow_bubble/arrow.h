#pragma once

#include <game_object.h>
#include <input_object.h>
#include <input.h>

#include <physics.h>

class arrow : public dynamic_object, public playable<arrow>, public physics<2>::physical_object {
public:
	arrow();
	~arrow() = default;
public:
	// dynamic_object overrides
	void init() override;
	void update(const scalar delta_time) override;
public:
	// playable_object overrides
	template <typename InputImpl>
	void process_input(InputImpl* input) {
		//m_velocity.zero();
		if (input->is_key_pressed(INPUT_KEY::KEY_W)) {
			/* Go up */
			push({ scalar(0), scalar(1), scalar(0) });
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_A)) {
			/* Go left */
			push({ scalar(-1), scalar(0), scalar(0) });
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_S)) {
			/* Go down */
			push({ scalar(0), scalar(-1), scalar(0) });
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_D)) {
			/* Go right */
			push({ scalar(1), scalar(0), scalar(0) });
		}

		constexpr geometry::scalar rotation_unit{ 0.05f };
		if (input->is_key_pressed(INPUT_KEY::KEY_LEFT)) {
			/* Rotate left */
			m_rotation += rotation_unit;
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_RIGHT)) {
			/* Rotate right */
			m_rotation += -rotation_unit;
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_UP)) {
			/* Rotate right */
			push({ geometry::cos(m_rotation), geometry::sin(m_rotation), 0.0f });
		}
		if (input->is_key_pressed(INPUT_KEY::KEY_DOWN)) {
			/* Rotate right */
			push({ -geometry::cos(m_rotation), -geometry::sin(m_rotation), 0.0f });
		}

		if (input->is_key_pressed(INPUT_KEY::KEY_SPACE)) {
			/* JUMP */
		}
	};
};