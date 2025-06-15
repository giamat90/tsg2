#include "physics.h"
#include <tsg/io.h>

using geometry::scalar;
using geometry::AXES;

physics::physics() {
	m_world = new physical_world();
};

physics::~physics() {
	delete m_world;
}

void physics::physical_world::compute() {
	/*	tsg::print("World box: bottom(x, y, z) = ({}, {}, {})\ttop(x, y, z) = ({}, {}, {})",
		m_limits.get_bottom().get_x(), m_limits.get_bottom().get_y(), m_limits.get_bottom().get_z(),
		m_limits.get_top().get_x(), m_limits.get_top().get_y(), m_limits.get_top().get_z());*/
	for (auto obj : m_objects) {
		//tsg::print("Object box: bottom(x, y, z) = ({}, {}, {})\ttop(x, y, z) = ({}, {}, {})",
		//	obj->m_box.get_bottom().get_x(), obj->m_box.get_bottom().get_y(), obj->m_box.get_bottom().get_z(),
		//	obj->m_box.get_top().get_x(), obj->m_box.get_top().get_y(), obj->m_box.get_top().get_z());
		auto translation = geometry::point3D();
		// compute if the new position is inside the world, else translate it
		if (obj->m_box.get_max(AXES::X) > m_limits.get_max(AXES::X)) {
			translation[AXES::X] -=  obj->m_box.get_max(AXES::X) - m_limits.get_max(AXES::X);
		}
		if (obj->m_box.get_max(AXES::Y) > m_limits.get_max(AXES::Y)) {
			translation[AXES::Y] -= obj->m_box.get_max(AXES::Y) - m_limits.get_max(AXES::Y);
		}
		if (obj->m_box.get_max(AXES::Z) > m_limits.get_max(AXES::Z)) {
			translation[AXES::Z] -= obj->m_box.get_max(AXES::Z) - m_limits.get_max(AXES::Z);
		}
		//
		if (obj->m_box.get_min(AXES::X) < m_limits.get_min(AXES::X)) {
			translation[AXES::X] += m_limits.get_min(AXES::X) - obj->m_box.get_min(AXES::X);
		}
		if (obj->m_box.get_min(AXES::Y) < m_limits.get_min(AXES::Y)) {
			translation[AXES::Y] += m_limits.get_min(AXES::Y) - obj->m_box.get_min(AXES::Y);
		}
		if (obj->m_box.get_min(AXES::Z) < m_limits.get_min(AXES::Z)) {
			translation[AXES::Z] += m_limits.get_min(AXES::Z) - obj->m_box.get_min(AXES::Z);
		}
		if (false == translation.is_zero()) {
			obj->translate(translation);
		}

	}

}

void physics::physical_object::update(const scalar delta_time) {
	if (m_angular_speed > 0.0f) {
		m_rotation += m_angular_speed * delta_time;
		m_box.rotate(m_rotation);
	}
	/* TODO: !!! */
	//m_position += m_position * m_linear_speed * delta_time;
	// p1 = p0 + p't + p''t^2/2
	auto candidate_position = m_position + m_velocity * delta_time + m_acceleration * delta_time * delta_time * 0.5f;
	// TODO: check if the new position is feasabile with the world
	//tsg::print("Pos = ({},{},{})", m_position.get_x(), m_position.get_y(), m_position.get_z());
	translate(candidate_position - m_position);
	//m_box.translate(m_position - candidate_position);
	//m_position = candidate_position;
}

void physics::physical_object::translate(const geometry::point3D& t) {
	m_box.translate(t);
	m_position = m_box.get_center();
}

void physics::physical_object::rotate(const scalar angle) {
	m_box.rotate(angle);
}



