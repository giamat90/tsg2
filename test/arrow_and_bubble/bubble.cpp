#include "bubble.h"
#include <sprite.h>
#include <tsg/logger.h>
#include <random>
#include <physics.h>
#include <contact.h>

bubble::bubble() : sprite_object(), physics<2>::physical_object() {}

void bubble::init() {
	m_sprite->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.png")).string());
	m_sprite->set_scale(0.5f);
	auto w = m_sprite->get_size().get<geometry::AXES::X>();
	auto h = m_sprite->get_size().get<geometry::AXES::Y>();
	// set random position	
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<scalar> dis(-1.0, 1.0);
	static std::vector<geometry::box2D> boxes;
	if(boxes.size() == 0) {
		auto b = geometry::box2D({ dis(gen), dis(gen) }, geometry::vector2D({ scalar(w) / scalar(2), scalar(h) / scalar(2) }).get_scalarized(m_world->get_scale()));
		boxes.emplace_back(b);
		set_bounding_volume<bounding_volume::type::aabb>(b.get_center(), b.get_half_sizes());
	}
	else {
		bool pos_available{ false };
		while (!pos_available) {
			scalar candidate_x = dis(gen);
			scalar candidate_y = dis(gen);
			auto candidate_box = geometry::box2D({ candidate_x, candidate_y }, geometry::vector2D({ scalar(w) / scalar(2), scalar(h) / scalar(2) }).get_scalarized(m_world->get_scale()));
			/* Help class friend to bubble to use protected method specialized for AABB... its for a good reason!*/
			class contact_friend : public contact_engine<2> {				
				friend bubble;
			};
			contact_friend engine;
			for (auto box : boxes) {
				engine.computeAABB(box, candidate_box);
				pos_available = !engine.has_contact();
			}
			if(pos_available) {
				set_bounding_volume<bounding_volume::type::aabb>(candidate_box.get_center(), candidate_box.get_half_sizes());
				boxes.emplace_back(candidate_box);
			}
		}
		if (!pos_available) {
			assert(0); // no position available
		}
	}
	tsg::logger::get_instance().write("Bubble start at ({},{}), (min_x, max_x) = ({}, {}), (min_y, max_y) = ({}, {})",
		boxes.back().get_center().get<geometry::AXES::X>(),
		boxes.back().get_center().get<geometry::AXES::Y>(),
		boxes.back().get_min(AXES::X),
		boxes.back().get_max(AXES::X),
		boxes.back().get_min(AXES::Y),
		boxes.back().get_max(AXES::Y));
	m_position = boxes.back().get_center();
	m_velocity = { dis(gen), dis(gen) };
	set_mass(scalar(1));
}

void bubble::update(const scalar delta_time) {
	physical_object::update(delta_time);
	sprite_object::update(m_position, m_rotation);
}