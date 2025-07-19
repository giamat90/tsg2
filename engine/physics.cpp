#include "physics.h"
#include <tsg/logger.h>
#include <tsg/io.h>

using geometry::scalar;
using geometry::AXES;
using geometry::vector3D;
using geometry::quaternion;

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
	/* First check if there is contact with world walls */
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
			/* Nothing to do for now */
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

bool physics::physical_world::contact(geometry::box3D& a, geometry::box3D& b) {
	/*
	* This is an overlapping std::size_t pre-test to detect early exiting from collision detection
	*/
	if (a.get_max(AXES::X) >= b.get_min(AXES::X) &&
		a.get_max(AXES::Y) >= b.get_min(AXES::Y) &&
		a.get_max(AXES::Z) >= b.get_min(AXES::Z)) 
	{
		bool collide = true;
		/* Store object's rotation matrix */
		auto a_rot = a.get_axes();
		auto b_rot = b.get_axes();
		/*
		* Axes that pass throw the two objects center.
		* Also known as translation axes
		*/
		vector3D axes{ b.get_center() - a.get_center() };
		/*
		* Compute the translation axes in a-obj's coord system
		*/
		axes = {
			vector3D::dot(axes, a_rot.get_row(AXES::X)),
			vector3D::dot(axes, a_rot.get_row(AXES::Y)),
			vector3D::dot(axes, a_rot.get_row(AXES::Z))
		};
		/*
		* Computing b-obj's rotation matrix in a-obj's coord system and its abs
		*/
		tsg::matrix<scalar, 3, 3> r{ tsg::matrix<scalar,3,3>::TYPE::ZERO };
		tsg::matrix<scalar, 3, 3> abs_r{ tsg::matrix<scalar,3,3>::TYPE::ZERO };
		for (std::size_t i = 0u; i < 3u; ++i) {
			for (std::size_t j = 0u; j < 3u; ++j) {
				r(i, j) = vector3D::dot(a_rot.get_row(i), b_rot.get_row(j));
				abs_r(i, j) = geometry::abs(r(i, j)) + geometry::epsilon;
			}
		}
		/*
		* Start with tests
		*/
		scalar ra{};
		scalar rb{};
		auto a_sizes = a.get_sizes();
		auto b_sizes = b.get_sizes();
		/*
		* Test axes A0, A1 and A2
		*/
		for (std::size_t i = 0u; i < 3u; ++i) {
			ra =
				a_sizes[AXES::X] * (i == 0 ? 1 : 0) +
				a_sizes[AXES::Y] * (i == 1 ? 1 : 0) +
				a_sizes[AXES::Z] * (i == 2 ? 1 : 0);
			rb =
				b_sizes[AXES::X] * abs_r(i, 0) +
				b_sizes[AXES::Y] * abs_r(i, 1) +
				b_sizes[AXES::Z] * abs_r(i, 2);
			const scalar val = (i == 0) ? axes[AXES::X] : (i == 1) ? axes[AXES::Y] : axes[AXES::Z];
			if (geometry::abs(val) > ra + rb) {
				return !collide;
			}
		}
		/*
		* Test axes B0, B1 and B2
		*/
		for (std::size_t i = 0u; i < 3u; ++i) {
			ra =
				a_sizes[AXES::X] * abs_r(i, 0) +
				a_sizes[AXES::Y] * abs_r(i, 1) +
				a_sizes[AXES::Z] * abs_r(i, 2);
			rb = (i == 0) ? b_sizes[AXES::X] :
				(i == 1) ? b_sizes[AXES::Y] : b_sizes[AXES::Z];
			const scalar prj =
				axes[AXES::X] * r(0, 1) +
				axes[AXES::Y] * r(1, i) +
				axes[AXES::Z] * r(2, i);
			if (geometry::abs(prj) > ra + rb) {
				return !collide;
			}
		}
		/*
		* Test all 9 cross product axes
		*/
		// A0 X B0
		ra = a_sizes[AXES::Y] * abs_r(2, 0) + a_sizes[AXES::Z] * abs_r(1, 0);
		rb = b_sizes[AXES::Y] * abs_r(0, 2) + a_sizes[AXES::Z] * abs_r(0, 1);
		if (geometry::abs(axes[AXES::Z] * r(1, 0) - axes[AXES::Y] * r(2, 0)) > ra + rb) {
			return !collide;
		}
		// A0 X B1
		ra = a_sizes[AXES::Y] * abs_r(2, 1) + a_sizes[AXES::Z] * abs_r(1, 1);
		rb = b_sizes[AXES::X] * abs_r(0, 2) + a_sizes[AXES::Z] * abs_r(0, 0);
		if (geometry::abs(axes[AXES::Z] * r(1, 1) - axes[AXES::Y] * r(2, 1)) > ra + rb) {
			return !collide;
		}
		// A0 X B2
		ra = a_sizes[AXES::Y] * abs_r(2, 2) + a_sizes[AXES::Z] * abs_r(1, 2);
		rb = b_sizes[AXES::X] * abs_r(0, 1) + a_sizes[AXES::Y] * abs_r(0, 0);
		if (geometry::abs(axes[AXES::Z] * r(1, 2) - axes[AXES::Y] * r(2, 2)) > ra + rb) {
			return !collide;
		}
		// A1 X B2
		ra = a_sizes[AXES::X] * abs_r(2, 0) + a_sizes[AXES::Z] * abs_r(0, 0);
		rb = b_sizes[AXES::Y] * abs_r(1, 2) + a_sizes[AXES::Z] * abs_r(1, 1);
		if (geometry::abs(axes[AXES::X] * r(2, 0) - axes[AXES::Z] * r(0, 0)) > ra + rb) {
			return !collide;
		}
		// A1 X B1
		ra = a_sizes[AXES::X] * abs_r(2, 1) + a_sizes[AXES::Z] * abs_r(0, 1);
		rb = b_sizes[AXES::Y] * abs_r(1, 2) + a_sizes[AXES::Z] * abs_r(1, 0);
		if (geometry::abs(axes[AXES::X] * r(2, 1) - axes[AXES::Z] * r(0, 1)) > ra + rb) {
			return !collide;
		}
		// A1 X B2
		ra = a_sizes[AXES::X] * abs_r(2, 2) + a_sizes[AXES::Z] * abs_r(0, 2);
		rb = b_sizes[AXES::X] * abs_r(1, 1) + a_sizes[AXES::Y] * abs_r(1, 0);
		if (geometry::abs(axes[AXES::X] * r(2, 2) - axes[AXES::Z] * r(0, 2)) > ra + rb) {
			return !collide;
		}
		// A2 X B0
		ra = a_sizes[AXES::X] * abs_r(1, 0) + a_sizes[AXES::Y] * abs_r(0, 0);
		rb = b_sizes[AXES::Y] * abs_r(2, 2) + a_sizes[AXES::Z] * abs_r(2, 1);
		if (geometry::abs(axes[AXES::Y] * r(0, 0) - axes[AXES::X] * r(1, 0)) > ra + rb) {
			return !collide;
		}
		// A2 X B1
		ra = a_sizes[AXES::X] * abs_r(1, 1) + a_sizes[AXES::Y] * abs_r(0, 1);
		rb = b_sizes[AXES::X] * abs_r(2, 2) + a_sizes[AXES::Z] * abs_r(2, 0);
		if (geometry::abs(axes[AXES::Y] * r(0, 1) - axes[AXES::X] * r(1, 1)) > ra + rb) {
			return !collide;
		}
		// A2 X B2
		ra = a_sizes[AXES::X] * abs_r(1, 2) + a_sizes[AXES::Y] * abs_r(0, 2);
		rb = b_sizes[AXES::X] * abs_r(2, 1) + a_sizes[AXES::Y] * abs_r(2, 0);
		if (geometry::abs(axes[AXES::Y] * r(0, 2) - axes[AXES::X] * r(1, 2)) > ra + rb) {
			return !collide;
		}
		/*
		* No separating axis found!!! Then there is a collision!
		*/
		return collide;
	} 
	else {
		return false;
	}
}

void physics::physical_world::resolve_contact(physics::physical_object* const a, physics::physical_object* const b) {
	/* 
	* Compute contact data
	*/
	geometry::obb_contact resolver(a->get_box(), b->get_box());
	resolver.compute();
	if (auto res = resolver.get_contact()) {
		auto contact = res.value();
		//contact.get_normal();
		//contact.get_point();
		/* I know that the box should translate along x of dx. The object has a velocity v that determine a direction
		* then to compute the point of contact I should translate the object of dx.
		*/
#if 1
		//if (!a->m_velocity.is_zero()) {
		//	scalar dx = a->get_box().get_max(AXES::X) - b->get_box().get_min(AXES::X);
		//	scalar dy{ dx * a->m_velocity.get<AXES::Y>() / a->m_velocity.get<AXES::X>() };
		//	scalar dz{ dx * a->m_velocity.get<AXES::Z>() / a->m_velocity.get<AXES::X>() };
		//	a->translate({ -dx, -dy, -dz });
		//}
		//else if (!b->m_velocity.is_zero()) {
		//	scalar dx = a->get_box().get_max(AXES::X) - b->get_box().get_min(AXES::X);
		//	scalar dy{ dx * b->m_velocity.get<AXES::Y>() / b->m_velocity.get<AXES::X>() };
		//	scalar dz{ dx * b->m_velocity.get<AXES::Z>() / b->m_velocity.get<AXES::X>() };
		//	b->translate({ -dx, -dy, -dz });
		//}
		//geometry::point3D point = a->get_box().get_max_point<AXES::X>();
		//geometry::vector3D normal = (a->get_box().get_center() - b->get_box().get_center()).get_normalized();
		geometry::point3D point = contact.get_normal();
		geometry::vector3D normal = contact.get_point();
		scalar seperataing_velocity{ vector3D::dot((a->m_velocity - b->m_velocity), normal) };
		scalar total_inverse_mass{ a->m_inverse_mass + b->m_inverse_mass };
		vector3D impulse{ (-2 * seperataing_velocity / total_inverse_mass) * normal };
		a->m_velocity += a->m_inverse_mass * impulse;
		b->m_velocity -= b->m_inverse_mass * impulse;
#endif
	}
	else {
		return;
	}

}

void physics::physical_object::update(const scalar delta_time) {
	tsg::logger::get_instance().write("{}: p=({},{},{}), v=({},{},{}), a=({},{},{})", this,
			m_position[geometry::AXES::X], m_position[geometry::AXES::Y], m_position[geometry::AXES::Z],
			m_velocity[geometry::AXES::X], m_velocity[geometry::AXES::Y], m_velocity[geometry::AXES::Z],
			m_acceleration[geometry::AXES::X], m_acceleration[geometry::AXES::Y], m_acceleration[geometry::AXES::Z]);
	/* 
	* Consume the acceleration due to forces 
	*/
	m_velocity += m_acceleration * delta_time;
	/*
	* Now the position changes due only the velocity. 
	* Update as a translation.
	*/
	translate(m_velocity * delta_time);
	/*
	* Clear accumulation
	*/
	m_acceleration.zero();
	/* 
	* Update orientation and rotation
	*/
#if 0
	quaternion delta_orientation{ 0, m_angular_velocity[AXES::X] * scalar(0.5), m_angular_velocity[AXES::Y] * scalar(0.5), m_angular_velocity[AXES::Z] * scalar(0.5) };
	m_orientation += delta_orientation * m_orientation;
	m_angular_velocity += m_angular_acceleration * delta_time;
#endif
	/*
	* Check if something goes very wrong
	*/
	if (std::isnan(m_position[AXES::X]) || std::isnan(m_position[AXES::X]) || std::isnan(m_position[AXES::X])) {
		tsg::logger::get_instance().write("Exception in {}: physical object computation fails to get a number.", __FILE__);
		throw;
	}
}

void physics::physical_object::translate(const geometry::point3D& scalar) {
	m_box.translate(scalar);
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
		//tsg::logger::get_instance().write("Is not possible set mass to zero.");
		throw;
	}
}




