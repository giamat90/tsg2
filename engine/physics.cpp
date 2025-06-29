#include "physics.h"
#include <tsg/io.h>

using geometry::scalar;
using geometry::AXES;
using geometry::vector3D;

physics::physics() {
	m_world = new physical_world();
};

physics::~physics() {
	delete m_world;
}

void physics::physical_world::compute() {
	/* Sorting objects x-based */
	std::sort(m_objects.begin(), m_objects.end(), [](physical_object* a, physical_object* b)
		{
			return a->get_box().get_min(geometry::X) < b->get_box().get_min(geometry::X);
		}
	);
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
		auto obj = *it;
		auto wall_contact = [&](const geometry::vector3D& normal) {
			scalar seperataing_velocity{ vector3D::dot((obj->m_velocity), normal) };
			vector3D impulse{ (-2 * seperataing_velocity / obj->m_inverse_mass) * normal };
			obj->m_velocity += obj->m_inverse_mass * impulse;
		};
		// compute if the new position is inside the world, else translate it
		if (obj->m_box.get_max(AXES::X) > m_limits.get_max(AXES::X)) {
			if (!obj->m_velocity.is_zero()) {
				scalar dx = obj->get_box().get_max(AXES::X) - m_limits.get_max(AXES::X);
				scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
				scalar dz{ dx * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::X>() };
				obj->translate({ -dx, -dy, -dz });
			}
			wall_contact({ scalar(-1), scalar(0), scalar(0) });
		}
		if (obj->m_box.get_max(AXES::Y) > m_limits.get_max(AXES::Y)) {
			if (!obj->m_velocity.is_zero()) {
				scalar dy = obj->get_box().get_max(AXES::Y) - m_limits.get_max(AXES::Y);
				scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
				scalar dz{ dy * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::Y>() };
				obj->translate({ -dx, -dy, -dz });
			}
			wall_contact({ scalar(0), scalar(-1), scalar(0) });
		}
		if (obj->m_box.get_max(AXES::Z) > m_limits.get_max(AXES::Z)) {

		}
		//
		if (obj->m_box.get_min(AXES::X) < m_limits.get_min(AXES::X)) {
			if (!obj->m_velocity.is_zero()) {
				scalar dx = obj->get_box().get_min(AXES::X) - m_limits.get_min(AXES::X);
				scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
				scalar dz{ dx * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::X>() };
				obj->translate({ -dx, -dy, -dz });
			}
			wall_contact({ scalar(1), scalar(0), scalar(0) });
		}
		if (obj->m_box.get_min(AXES::Y) < m_limits.get_min(AXES::Y)) {
			if (!obj->m_velocity.is_zero()) {
				scalar dy = obj->get_box().get_min(AXES::Y) - m_limits.get_min(AXES::Y);
				scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
				scalar dz{ dy * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::Y>() };
				obj->translate({ -dx, -dy, -dz });
			}
			wall_contact({ scalar(0), scalar(1), scalar(0) });
		}
		if (obj->m_box.get_min(AXES::Z) < m_limits.get_min(AXES::Z)) {
			if (!obj->m_velocity.is_zero()) {
				scalar dz = obj->get_box().get_min(AXES::Z) - m_limits.get_min(AXES::Z);
				scalar dx{ dz * obj->m_velocity.get<AXES::X>() / obj->m_velocity.get<AXES::Z>() };
				scalar dy{ dz * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Z>() };
				obj->translate({ -dx, -dy, -dz });
			}
			wall_contact({ scalar(0), scalar(0), scalar(1) });
		}

		/* Search contacts with next objects */
		auto next_it = std::next(it);
		while (next_it != m_objects.end()) {
			auto next_obj = *next_it;
			if (contact(obj->get_box(), next_obj->get_box())) {
				resolve_contact(obj, next_obj);
				++next_it;
			}
			else {
				next_it = m_objects.end();
			}
		}
	}
}

bool physics::physical_world::contact(geometry::box3D a, geometry::box3D b) {
	if (a.get_max(AXES::X) >= b.get_min(AXES::X)) {
		if (a.get_max(AXES::Y) >= b.get_min(AXES::Y) ||
			a.get_min(AXES::Y) >= b.get_max(AXES::Y) ||
			a.get_max(AXES::Z) >= b.get_min(AXES::Z) ||
			a.get_min(AXES::Z) >= b.get_max(AXES::Z))
		{
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void physics::physical_world::resolve_contact(physics::physical_object* const a, physics::physical_object* const b) {
	/* I know that the box should translate along x of dx. The object has a velocity v that determine a direction
	* then to compute the point of contact I should translate the object of dx.
	*/
	auto a0 = a->m_position;
	auto b0 = b->m_position;
	auto va0 = a->m_velocity;
	auto vb0 = b->m_velocity;
	if (!a->m_velocity.is_zero()) {
		scalar dx = a->get_box().get_max(AXES::X) - b->get_box().get_min(AXES::X);
		scalar dy{ dx * a->m_velocity.get<AXES::Y>() / a->m_velocity.get<AXES::X>() };
		scalar dz{ dx * a->m_velocity.get<AXES::Z>() / a->m_velocity.get<AXES::X>() };
		a->translate({ -dx, -dy, -dz });
	}
	else if (!b->m_velocity.is_zero()) {
		scalar dx = a->get_box().get_max(AXES::X) - b->get_box().get_min(AXES::X);
		scalar dy{ dx * b->m_velocity.get<AXES::Y>() / b->m_velocity.get<AXES::X>() };
		scalar dz{ dx * b->m_velocity.get<AXES::Z>() / b->m_velocity.get<AXES::X>() };
		b->translate({ -dx, -dy, -dz });
	}
	geometry::point3D point = a->get_box().get_max_point<AXES::X>();
	geometry::vector3D normal = (a->get_box().get_center() - b->get_box().get_center()).get_normalized();
	scalar seperataing_velocity{ vector3D::dot((a->m_velocity - b->m_velocity), normal) };
	scalar total_inverse_mass{ a->m_inverse_mass + b->m_inverse_mass };
	vector3D impulse{ (-2 * seperataing_velocity / total_inverse_mass) * normal };
	a->m_velocity += a->m_inverse_mass * impulse;
	b->m_velocity -= b->m_inverse_mass * impulse;
}

void physics::physical_object::update(const scalar delta_time) {
	if (m_angular_speed > 0.0f) {
		m_rotation += m_angular_speed * delta_time;
		m_box.rotate(m_rotation);
	}
	// consume the acceleration due forces
	m_velocity += m_acceleration * delta_time;
	// p1 = p0 + p't + p''t^2/2
	geometry::point3D new_position = m_position + m_velocity * delta_time;
	translate(new_position - m_position);
	m_acceleration.zero();
	if (std::isnan(m_position[AXES::X]) || std::isnan(m_position[AXES::X]) || std::isnan(m_position[AXES::X])) {
		tsg::print("Ooops! physical object computation fails to get a number.");
		throw;
	}
}

void physics::physical_object::translate(const geometry::point3D& t) {
	m_box.translate(t);
	m_position = m_box.get_center();
}

void physics::physical_object::rotate(const scalar angle) {
	m_box.rotate(angle);
}

void physics::physical_object::set_mass(const scalar m) {
	if (m > scalar(0)) {
		m_inverse_mass = scalar(1) / m;
	}
	else {
		tsg::print("Is not possible set mass to zero.");
		throw;
	}
}




