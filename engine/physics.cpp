#include "physics.h"

void physics::physical_object::update(const float delta_time) {
	if (m_angular_speed > 0.0f) {
		m_rotation += m_angular_speed * delta_time;
	}
	if (m_linear_speed > 0.0f) {
		/* TODO: !!! */
		//m_position += m_position * m_linear_speed * delta_time;
		m_position += geometry::point3D(m_linear_speed * delta_time);
	}
	// TODO: check if the new position is feasabile with the world
}

