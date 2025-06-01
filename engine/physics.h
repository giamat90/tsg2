#pragma once

#include "tsg2.h"
#include "game_object.h"
#include <geometry.h>
#include <vector>

class TSG2_API physics {
	/* Support classes */
public:
	/* Class of physical world that determine limits and other stuffs */
	class physical_world {
	public:
		physical_world() = default;
		~physical_world() = default;
	protected:
		geometry::rectangle m_limits;
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
		void update(const float delta_time) override;
	protected:
		physical_world* m_world{ nullptr };
		geometry::point3D m_position{};
		//geometry::point3D m_rotation{};
		geometry::scalar m_rotation{};
		geometry::scalar m_linear_speed{};
		geometry::scalar m_angular_speed{};
		geometry::scalar m_direction{};
		geometry::scalar m_normal{};
	};
public:
	/* ctors and dtors methods */
	physics() = default;
	~physics() = default;
public:
	inline void update(const float delta_time) {
		for (auto o : m_physical_object) {
			o->update(delta_time);
		}
	}
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_physical_object(physical_object* o) {
		o->m_world = m_world;
		m_physical_object.push_back(o);
	}
protected:
	physical_world* m_world{ nullptr };
	std::vector<physical_object*> m_physical_object;
};

