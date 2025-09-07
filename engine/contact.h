#pragma once

#include "geometry.h"
// std
#include <variant>
#include <map>
#include <functional>

using geometry::scalar;
using geometry::AXES;

template <std::size_t Dim> requires geometry::GeometricDimension<Dim>
class contact_engine {
	using vector = tsg::vector<scalar, Dim>;
	using point = tsg::vector<scalar, Dim>;
	using box = geometry::box<Dim>;
	using sphere = geometry::sphere<Dim>;
public:
	enum class box_resolver {
		AABB,
		OBB,
		UNKNOW
	};
public:
	contact_engine(box_resolver t = box_resolver::UNKNOW) : m_box_resolver(t) {};
	~contact_engine() = default;
public:
	bool resolve(geometry::bounding_volume * const first, geometry::bounding_volume * const second) {
		const auto it = m_resolve_type_map.find(std::make_pair(first->get_type(), second->get_type()));
		if (it != m_resolve_type_map.end()) {
			it->second(*first, *second);
		}
		else {
			assert(0);
		}
		return m_has_contact;
	}
public:
	inline bool has_contact() const { return m_has_contact; }
public:
	/* getters result of computation */
	inline point get_point() const { return m_point; }
	inline vector get_normal() const { return m_normal; }
	inline scalar get_penetration() const { return m_penetration; }
	inline vector get_penetration_vector() const { return m_penetration_vector; }
protected:
	void compute(box& first, box& second) {
		/* AABB test */
		switch (m_box_resolver)
		{
		case(box_resolver::AABB):
			computeAABB(first, second);
			break;
		case(box_resolver::OBB):
			computeOBB(first, second);
			break;
		default:
			/* unknown box resolver */
			assert(0); 
			break;
		}
	}
	void compute(box& fisrt, sphere& second){
		/* TODO */
		assert(0);
	}; 
	void compute(sphere& first, sphere& second) {
		scalar sphere_distance{ point(first.get_center() - second.get_center()).get_norm() };
		scalar radius_sum{ first.get_radius() + second.get_radius() };
		if (tsg::pow(sphere_distance, 2) <= tsg::pow(radius_sum, 2)) {
			/* collision */
			/* TODO: resolve collision */
			m_has_contact = true;
			assert(0);
		}
		else {
			/* no collision */
			/* TODO */
			assert(0);
		}
	}
	void computeAABB(box& first, box& second) {
		if constexpr (Dim == 2) {
			if (first.get_max(AXES::X) > second.get_min(AXES::X)) {
				if ((first.get_max(AXES::Y) > second.get_min(AXES::Y)) &&
					(second.get_max(AXES::Y) > first.get_min(AXES::Y)))
				{
					/* collision! */
					/* TODO: resolve collision */
					m_has_contact = true;/* valid for AABB bounding volumes */
					scalar penetration_x = std::min(
						first.get_max(AXES::X) - second.get_min(AXES::X),
						second.get_max(AXES::X) - first.get_min(AXES::X)
					);
					scalar penetration_y = std::min(
						first.get_max(AXES::Y) - second.get_min(AXES::Y),
						second.get_max(AXES::Y) - first.get_min(AXES::Y)
					);
					/* Choose the minimum axes penetration */
					if (penetration_x < penetration_y) {
						/* X axes */
						m_penetration = penetration_x;
						m_normal = first.get_center()[AXES::X] < second.get_center()[AXES::X] ?
							vector({ scalar(-1), scalar(0) }) : vector({ scalar(1), scalar(0) });
						m_penetration_vector = vector({ m_normal[AXES::X] * m_penetration, scalar(0) });
					}
					else {
						/* Y axes */
						m_penetration = penetration_y;
						m_normal = first.get_center()[AXES::Y] < second.get_center()[AXES::Y] ?
							vector({ scalar(0), scalar(-1) }) : vector({ scalar(0), scalar(1) });
						m_penetration_vector = vector({ m_normal[AXES::Y] * m_penetration, scalar(0) });

					}
					m_point = (first.get_center() + second.get_center()) * scalar(0.5);
				}
			}
			else {
				return; // no collision
			}
		}
		else if constexpr (Dim == 3) {
			assert(0); // TODO
		}
		else {
			/* Not allowed dimension */
			assert(0); 
		}
	}
	void computeOBB(box& a, box& b) {
		if constexpr (Dim == 2) {
			/* TODO */
			assert(0);
		}
		else if constexpr (Dim == 3) {
			/* TODO */
			assert(0);
		}
		else {
			/* Not allowed dimension */
			assert(0);

		}
	}
private:
	box_resolver m_box_resolver{ box_resolver::UNKNOW };
	bool m_has_contact{ false };
	contact_engine::point m_point{};
	contact_engine::vector m_normal{};
	scalar m_penetration{};
	vector m_penetration_vector{};
	std::map< 
		std::pair<geometry::bounding_volume::type, geometry::bounding_volume::type>, 
		std::function<void(geometry::bounding_volume&, geometry::bounding_volume&)>> m_resolve_type_map
	{
		{
			{geometry::bounding_volume::type::box, geometry::bounding_volume::type::box},
			[&](geometry::bounding_volume& l, geometry::bounding_volume& r) -> void { compute(static_cast<box&>(l), static_cast<box&>(r)); }
		},
		{
			{geometry::bounding_volume::type::box, geometry::bounding_volume::type::sphere},
			[&](geometry::bounding_volume& l, geometry::bounding_volume& r) -> void { compute(static_cast<box&>(l), static_cast<sphere&>(r)); }
		},
		{
			{geometry::bounding_volume::type::sphere, geometry::bounding_volume::type::sphere},
			[&](geometry::bounding_volume& l, geometry::bounding_volume& r) -> void { compute(static_cast<sphere&>(l), static_cast<sphere&>(r)); }
		}
	};
};