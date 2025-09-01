#pragma once

#include "tsg2.h"
#include "game_object.h"
#include <geometry.h>
#include <vector>
#include <algorithm>
#include <tsg/io.h>
#include <utility>
#include <tsg/types.h>

using geometry::AXES;
using geometry::scalar;
using geometry::quaternion;
using geometry::matrix3D;
using geometry::bounding_volume;

#define RESOLVE_INTERPENETRATION 0

template <std::size_t Dim> requires geometry::GeometricDimension<Dim>
class physics {
	/* Generic definitions */
	/* TODO: parametrize the size*/
	using vector = tsg::vector<scalar, Dim>;
	using point = tsg::vector<scalar, Dim>;
	using box = geometry::box<Dim>;
public:
	/* Support classes */
	/* Class of physical world that determine limits, determine contacs and resolve them. */
	class physical_object; // forward declaration
	class physical_world {
		friend physics;
	public:
		physical_world() = default;
		~physical_world() = default;
	public:
		void compute() {
			if constexpr (Dim == 3) {
				/* Sorting objects x-based */
				std::sort(m_objects.begin(), m_objects.end(), [](physical_object* a, physical_object* b)
					{
						return a->get_box().get_min(geometry::X) < b->get_box().get_min(geometry::X);
					}
				);
				/* First check if there is contact with world walls */
				for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
					auto obj = *it;
					auto wall_contact = [&](const vector& normal) {
							scalar seperataing_velocity{ vector::dot((obj->m_velocity), normal) };
							vector impulse{ (-2 * seperataing_velocity / obj->m_inverse_mass) * normal };
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
			else if constexpr (Dim == 2) {
				constexpr std::size_t max_iterations{ 1 };
				for (std::size_t iteration{}; iteration < max_iterations; ++iteration) {
					tsg::logger::get_instance().write("Iteration {}", iteration);
					/* Sorting objects x-based */
					std::sort(m_objects.begin(), m_objects.end(), [](physical_object* a, physical_object* b) 
						{
							return a->get_box().get_min(geometry::X) < b->get_box().get_min(geometry::X);
						}
					);
					/* First check if there is contact with world walls */
					for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
						auto obj = *it;
						auto wall_contact = [&](const vector& normal) {
							scalar seperataing_velocity{ vector::dot((obj->m_velocity), normal) };
							vector impulse{ (-2 * seperataing_velocity / obj->m_inverse_mass) * normal };
							obj->m_velocity += obj->m_inverse_mass * impulse;
							//obj->update(scalar(0));
							};
						// compute if the new position is inside the world, else translate it
						if (obj->m_box.get_max(AXES::X) > m_limits.get_max(AXES::X)) {
							if (!obj->m_velocity.is_zero()) {
								scalar dx = obj->get_box().get_max(AXES::X) - m_limits.get_max(AXES::X);
								scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
								obj->translate({ -dx, -dy });
							}
							wall_contact({ scalar(-1), scalar(0) });
							return;
						}
						if (obj->m_box.get_max(AXES::Y) > m_limits.get_max(AXES::Y)) {
							if (!obj->m_velocity.is_zero()) {
								scalar dy = obj->get_box().get_max(AXES::Y) - m_limits.get_max(AXES::Y);
								scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
								obj->translate({ -dx, -dy });
							}
							wall_contact({ scalar(0), scalar(-1) });
							return;
						}
						//
						if (obj->m_box.get_min(AXES::X) < m_limits.get_min(AXES::X)) {
							if (!obj->m_velocity.is_zero()) {
								scalar dx = obj->get_box().get_min(AXES::X) - m_limits.get_min(AXES::X);
								scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
								obj->translate({ -dx, -dy });
							}
							wall_contact({ scalar(1), scalar(0) });
							return;
						}
						if (obj->m_box.get_min(AXES::Y) < m_limits.get_min(AXES::Y)) {
							if (!obj->m_velocity.is_zero()) {
								scalar dy = obj->get_box().get_min(AXES::Y) - m_limits.get_min(AXES::Y);
								scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
								obj->translate({ -dx, -dy });
							}
							wall_contact({ scalar(0), scalar(1) });
							return;
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
			}
			else {
				assert(0);
			}
		};
		vector get_scale() { return m_scale; }
	protected:
		bool contact(box& a, box& b) {
			if constexpr (Dim == 3) {
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
					vector axes{ b.get_center() - a.get_center() };
					/*
					* Compute the translation axes in a-obj's coord system
					*/
					axes = {
						vector::dot(axes, a_rot.get_row(AXES::X)),
						vector::dot(axes, a_rot.get_row(AXES::Y)),
						vector::dot(axes, a_rot.get_row(AXES::Z))
					};
					/*
					* Computing b-obj's rotation matrix in a-obj's coord system and its abs
					*/
					tsg::matrix<scalar, 3, 3> r{ tsg::matrix<scalar,3,3>::TYPE::ZERO };
					tsg::matrix<scalar, 3, 3> abs_r{ tsg::matrix<scalar,3,3>::TYPE::ZERO };
					for (std::size_t i = 0u; i < 3u; ++i) {
						for (std::size_t j = 0u; j < 3u; ++j) {
							r(i, j) = vector::dot(a_rot.get_row(i), b_rot.get_row(j));
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
			else if constexpr (Dim == 2) {
				/* AABB test */
				if (a.get_max(AXES::X) > b.get_min(AXES::X)) {
					if ((a.get_max(AXES::Y) > b.get_min(AXES::Y)) &&
						(b.get_max(AXES::Y) > a.get_min(AXES::Y)))
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
#if 0
				/* OBB test */ /
				auto is_separating_axis = [](const vector& axis, const box& a, const box& b) -> scalar {
					auto project_box_into_axes = [](const box& b, const vector& axis) -> std::pair<scalar, scalar> {
						scalar center_proj = vector::dot(b.get_center(), axis);
						scalar radius =
							geometry::abs(b.get_size<AXES::X>() * vector::dot(b.get_axes().get_row<AXES::X>(), axis)) +
							geometry::abs(b.get_size<AXES::Y>() * vector::dot(b.get_axes().get_row<AXES::Y>(), axis));
						return { center_proj - radius, center_proj + radius };
					};
					auto [min_a, max_a] = project_box_into_axes(a, axis);
					auto [min_b, max_b] = project_box_into_axes(b, axis);
					return (max_a < min_b - geometry::scalar_zero) || (max_b < min_a - geometry::scalar_zero);
				};

				for (size_t i{}; i < 2u; ++i) {
					if (is_separating_axis(a.get_axes().get_row(i), a, b) || is_separating_axis(b.get_axes().get_row(i), a, b)) {
						return false;
					}
				}
				/* no separating axes found, then there is a collision */
				return true;
#endif
			}
			else {
				assert(0);
			}
		};
		void resolve_contact(physics::physical_object * const a, physics::physical_object * const b) {
			/*
			* Compute contact data
			*/
#if 1
			geometry::obb_contact<Dim> resolver(a->get_box(), b->get_box());
			resolver.compute();
			if (auto res = resolver.get_best_contact()) {
				auto contact = res.value();
				scalar total_inverse_mass{ a->m_inverse_mass + b->m_inverse_mass };
#if RESOLVE_INTERPENETRATION
				/* resolve interpenetration */
				vector move_per_mass{ contact.get_penetration_vector() * (scalar(-1) / total_inverse_mass) };
				a->translate(a->m_inverse_mass * move_per_mass);
				b->translate(b->m_inverse_mass * move_per_mass);
#endif
				/* resolve velocity */
				scalar separating_velocity{ vector::dot((a->m_velocity - b->m_velocity), contact.get_normal()) };
				if(separating_velocity > scalar(0)) {
					// objects are separating, no need to resolve
					return;
				}		
				/* Linear coefficient */
				/* case with restitution coeff variable */
				constexpr scalar restutition_coeff{ scalar(1) };
				scalar new_separating_velocity{ -separating_velocity * restutition_coeff };
				scalar delta_velocity{ new_separating_velocity - separating_velocity };
				vector impulse{ (delta_velocity / total_inverse_mass) * contact.get_normal() };
				/* case in which I don't have restitution degradation (restitution coeff = 1) */
				vector old_impulse{ (-2 * separating_velocity / total_inverse_mass) * contact.get_normal()};
				a->m_velocity += a->m_inverse_mass * impulse;
				b->m_velocity -= b->m_inverse_mass * impulse;
				/* Angular coefficient */
				if constexpr (Dim == 3) {
					assert(0); // TODO
				} else if constexpr (Dim == 2) {
					auto tmp = contact.get_point() - a->m_position;
					auto opposite_impulse = scalar(-1) * impulse;
					const scalar torque_a = tsg::vector<scalar, 3>::cross(geometry::point3D(tmp), geometry::point3D(opposite_impulse))[AXES::Z];
					tmp = contact.get_point() - b->m_position;
					const scalar torque_b = tsg::vector<scalar, 3>::cross(geometry::point3D(tmp), geometry::point3D(opposite_impulse))[AXES::Z];
					a->m_angular_velocity += torque_a * a->m_inverse_mass * impulse.get<AXES::Z>();
					b->m_angular_velocity -= torque_b * b->m_inverse_mass * impulse.get<AXES::Z>();
				}
			}
#endif
		};
	protected:
		std::vector<physical_object*> m_objects;
		box m_limits;
		vector m_scale;
		vector m_forces;
	};
	/* physical_object to compute motion and collisions */
	class physical_object {
		friend physics;
	public:
	public:
		void set_physical_world(physical_world* const world) { m_world = world; }
	public:
		// updateable method overrides
		virtual void update(const scalar delta_time) {
			if constexpr (Dim == 2) {
				tsg::logger::get_instance().write("{}: p=({},{}), v=({},{}), a=({},{})", this,
					m_position[geometry::AXES::X], m_position[geometry::AXES::Y],
					m_velocity[geometry::AXES::X], m_velocity[geometry::AXES::Y],
					m_acceleration[geometry::AXES::X], m_acceleration[geometry::AXES::Y]);
			}
			else if constexpr (Dim == 3) {
				tsg::logger::get_instance().write("{}: p=({},{},{}), v=({},{},{}), a=({},{},{})", this,
					m_position[geometry::AXES::X], m_position[geometry::AXES::Y], m_position[geometry::AXES::Z],
					m_velocity[geometry::AXES::X], m_velocity[geometry::AXES::Y], m_velocity[geometry::AXES::Z],
					m_acceleration[geometry::AXES::X], m_acceleration[geometry::AXES::Y], m_acceleration[geometry::AXES::Z]);
			}
			else {
				assert(0);
			}
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
#if TODO
			quaternion delta_orientation{ 0, m_angular_velocity[AXES::X] * scalar(0.5), m_angular_velocity[AXES::Y] * scalar(0.5), m_angular_velocity[AXES::Z] * scalar(0.5) };
			m_orientation += delta_orientation * m_orientation;
			m_angular_velocity += m_angular_acceleration * delta_time;
#endif
			/*
			* Check if something goes very wrong
			*/
			if (m_position.is_nan()) {
				tsg::logger::get_instance().write("Exception in {}: physical object computation fails to get a number.", __FILE__);
				assert(0);
			}
		};
	public:
		void translate(const point& t) {
			m_box.translate(t);
			m_position = m_box.get_center();
		}
		void rotate(const scalar angle) {
			m_box.rotate(angle);
		}
	public:
		/* TODO: change name in set_bounding_volume it is more generic and correct and it should
		* accept boxes, spheres as minimun and other more complicated volumes also.
		*/
		void set_bounding_volume(bounding_volume& bv) {
			switch (bv.get_type()) {
			case bounding_volume::type::box:
			{
				vector half_sizes = static_cast<box&>(bv).get_half_sizes();
				half_sizes.scale(m_world->get_scale());
				m_box = box(static_cast<box&>(bv).get_center(), half_sizes);
			}
			break;
			case bounding_volume::type::sphere:
			{
				/* TODO */
				assert(0);
			}
			break;
			default:
				assert(0);
				break;	
			}
		};
		bounding_volume * const get_bounding_volume() { return &m_box; }
		box& get_box() { return m_box; }
		void set_mass(const scalar m) {
			if (m > scalar(0)) {
				m_inverse_mass = scalar(1) / m;
			}
			else {
				assert(false);
			}
		};
		scalar get_mass() const { return scalar(1) / m_inverse_mass; }
		void set_infinite_mass() { m_inverse_mass = scalar(0); }
	protected:
		inline void push(const vector& force) {
			m_acceleration += m_inverse_mass * force;
		}
	protected:
		physical_world* m_world{ nullptr };
		// linear proprieties
		vector m_position{};
		vector m_velocity{};
		vector m_acceleration{};
		scalar m_inverse_mass{scalar(1)};
		// angular proprietis
		quaternion m_orientation{};
		vector m_angular_velocity{};
		vector m_angular_acceleration{};
		matrix3D m_inverse_intertia_tensor{};
		scalar m_rotation{};
		scalar m_angular_speed{};
		// accumulators
		vector m_forces{};
		vector m_torques{};
		vector m_impulse{};
		// dampings
		scalar m_linear_damping{};
		scalar m_angular_damping{};
		// boundaries
		box m_box;
	};
public:
	/* ctors and dtors methods */
	physics() {
		m_world = new physical_world();
	};
	~physics() {
		delete m_world;
	}
public:
	// set proprieties
	void set_limits(const vector& half_sizes, const scalar scale = scalar(1.0f)) {
		/*
		* in 3D case: { 1.0f / (0.5f * width) , 1.0f / (0.5f * height), 1.0f / (0.5f * depth) } 
		* in 2D case: { 1.0f / (0.5f * width) , 1.0f / (0.5f * height) } 
		*/
		if (auto res = half_sizes.reciprocal()) {
			m_world->m_scale = res.value() * scalar(2.0f) * scale;
		}
		else {
			assert(0);
		}
		/* setting world limits */
		m_world->m_limits.set_center(vector(scalar(0)));
		m_world->m_limits.set_half_sizes(vector(scale));
#if _DEBUG
		tsg::logger::get_instance().write("World limits: ({},{},{},{}) with scale ({},{})", 
			m_world->m_limits.get_center()[AXES::X] - m_world->m_limits.get_sizes()[AXES::X],
			m_world->m_limits.get_center()[AXES::X] + m_world->m_limits.get_sizes()[AXES::X],
			m_world->m_limits.get_center()[AXES::Y] + m_world->m_limits.get_sizes()[AXES::Y],
			m_world->m_limits.get_center()[AXES::Y] - m_world->m_limits.get_sizes()[AXES::Y],
			m_world->m_scale[AXES::X],
			m_world->m_scale[AXES::Y]);
#endif
	}
public:
	inline void update(const float delta_time) {
		for (auto o : m_physical_object) {
			o->update(delta_time);
		}
		m_world->compute();
	}
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_physical_object(physical_object* o) {
		o->m_world = m_world;
		// TODO: leave only once, it is a wasteful repetition
		m_physical_object.push_back(o);
		m_world->m_objects.push_back(o);
	}
protected:
	physical_world* m_world{ nullptr };
	std::vector<physical_object*> m_physical_object;
};

