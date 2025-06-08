#include "physics.h"
#include <tsg/io.h>

void physics::physical_object::update(const float delta_time) {
	if (m_angular_speed > 0.0f) {
		m_rotation += m_angular_speed * delta_time;
	}
	/* TODO: !!! */
	//m_position += m_position * m_linear_speed * delta_time;
	m_position += geometry::point3D(m_linear_speed * delta_time);
	// TODO: check if the new position is feasabile with the world
	//tsg::print("Pos = ({},{},{})", m_position.get_x(), m_position.get_y(), m_position.get_z());
}

