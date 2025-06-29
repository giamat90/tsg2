#pragma once

#include "tsg2.h"
#include "game_object.h"
#include <geometry.h>
#include <logger.h>
#include <vector>
#include <algorithm>

using geometry::scalar;

class TSG2_API physics {
	/* Support classes */
public:
	/* Class of physical world that determine limits, determine contacs and resolve them. */
	class physical_object; // forward declaration
	class TSG2_API physical_world {
		friend physics;
	public:
		physical_world() = default;
		~physical_world() = default;
	public:
		void compute();
		geometry::vector<2> get_scale() { return m_scale; }
	protected:
		bool contact(geometry::box3D a, geometry::box3D b);
		void resolve_contact(physics::physical_object * const a, physics::physical_object * const b);
	protected:
		std::vector<physical_object*> m_objects;
		geometry::box3D m_limits;
		geometry::vector<2> m_scale;
		geometry::vector<3> m_forces;

	};
	/* physical_object to compute motion and collisions */
	class TSG2_API physical_object : public updateable {
		friend physics;
	public:
		physical_object() = default;
		virtual ~physical_object() = default;
	public:
		void set_physical_world(physical_world* const world) { m_world = world; }
	public:
		// updateable method overrides
		void update(const geometry::scalar delta_time) override;
	public:
		void translate(const geometry::point3D& t);
		void rotate(const geometry::scalar angle);
	public:
		void set_box(const geometry::point3D& a, const geometry::point3D& b) {
			m_box = 
			{
				{m_world->get_scale().get_x() * a.get_x(), m_world->get_scale().get_y() * a.get_y(), scalar(0)},
				{m_world->get_scale().get_x() * b.get_x(), m_world->get_scale().get_y() * b.get_y(), scalar(0)}
			};
		}
		geometry::box3D get_box() const { return m_box; }
		void set_mass(const scalar m);
		scalar get_mass() const { return scalar(1) / m_inverse_mass; }
		void set_infinite_mass() { m_inverse_mass = scalar(0); }
	protected:
		inline void push(const geometry::vector3D& force) {
			m_acceleration += m_inverse_mass * force;
		}
	protected:
		physical_world* m_world{ nullptr };
		geometry::point3D m_position{};
		geometry::point3D m_velocity{};
		geometry::point3D m_acceleration{};
		geometry::scalar m_rotation{};
		geometry::box3D m_box;
		geometry::scalar m_angular_speed{};
		geometry::scalar m_inverse_mass{scalar(1)};
		geometry::point3D m_forces{};
		geometry::point3D m_torques{};
		geometry::point3D m_impulse{};
	};
public:
	/* ctors and dtors methods */
	physics();
	~physics();
public:
	// set proprieties
	void set_limits(const geometry::scalar width, const geometry::scalar height, const geometry::scalar scale = 1.0f) {
		m_world->m_scale = { 1.0f / (0.5f * width) , 1.0f / (0.5f * height) };
		m_world->m_limits = { { -1.0f * scale, -1.0f * scale, 0.0f * scale}, {1.0f * scale, 1.0f * scale, 0.0f * scale} };
	}
public:
	inline void update(const float delta_time) {
		for (auto o : m_physical_object) {
			o->update(delta_time);
		}
		m_world->compute();

		for (auto o : m_physical_object) {
			logger::get_istance().write("{}:\tp=({},{},{}),\tv=({},{},{}),\ta=({},{},{})", o,
				o->m_position[geometry::AXES::X], o->m_position[geometry::AXES::Y], o->m_position[geometry::AXES::Z],
				o->m_velocity[geometry::AXES::X], o->m_velocity[geometry::AXES::Y], o->m_velocity[geometry::AXES::Z],
				o->m_acceleration[geometry::AXES::X], o->m_acceleration[geometry::AXES::Y], o->m_acceleration[geometry::AXES::Z]);
		}
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

