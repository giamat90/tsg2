#include "bubble.h"
#include <sprite.h>
#include <tsg/logger.h>
#include <random>
#include <physics.h>

bubble::bubble() {
	m_sprite = sprite::create_sprite();
}



void bubble::init() {
	m_sprite->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\bubble.png")).string());
	m_sprite->set_scale(0.5f);
	auto w = m_sprite->get_size().get<geometry::AXES::X>();
	auto h = m_sprite->get_size().get<geometry::AXES::Y>();
	// set random position	
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<scalar> dis(-1.0, 1.0);
#if 0
	auto b = geometry::box2D({ dis(gen), dis(gen) }, { scalar(w) / scalar(2), scalar(h) / scalar(2) });
	set_bounding_volume(b);
#else
	static std::vector<geometry::box2D> boxes;
	if(boxes.size() == 0) {
		auto b = geometry::box2D({ dis(gen), dis(gen) }, { scalar(w) / scalar(2), scalar(h) / scalar(2) });
		set_bounding_volume(b);
		auto half_sizes = { m_box.get_half_sizes()[AXES::X], m_box.get_half_sizes()[AXES::Y] };
		b.set_half_sizes(half_sizes);
		boxes.emplace_back(b);
	}
	else {
		bool pos_available{ false };
		while (!pos_available) {
			scalar candidate_x = dis(gen);
			scalar candidate_y = dis(gen);
			auto candidate_box = geometry::box2D({ candidate_x, candidate_y }, { scalar(w) / scalar(2), scalar(h) / scalar(2) });
			auto copy = candidate_box; /* need a copy because of scale inside set_bounding_volume */
			auto half_sizes = { candidate_box.get_half_sizes()[AXES::X] * m_world->get_scale()[AXES::X], candidate_box.get_half_sizes()[AXES::Y] * m_world->get_scale()[AXES::Y] };
			candidate_box.set_half_sizes(half_sizes);
			auto compute_overlap = [](const geometry::box2D& a, const geometry::box2D& b) -> bool 
				{
					bool overlap = true;
					if (a.get_max(AXES::X) < b.get_min(AXES::X) || a.get_max(AXES::Y) < b.get_min(AXES::Y)) {
						overlap = false;
					}
					return overlap;
				};
			for (auto box : boxes) {
				pos_available = !(compute_overlap(box, candidate_box) && compute_overlap(candidate_box, box));
			}
			if(pos_available) {
				set_bounding_volume(copy);
				boxes.emplace_back(candidate_box);
			}
		}
		if (!pos_available) {
			assert(0); // no position available
		}
	}
#endif
	tsg::logger::get_instance().write("Bubble start at ({},{})",
		m_box.get_center().get<geometry::AXES::X>(),
		m_box.get_center().get<geometry::AXES::Y>());
	set_mass(scalar(1));
	m_velocity = { dis(gen), dis(gen) };
}

void bubble::update(const scalar delta_time) {
	physical_object::update(delta_time);
	m_sprite->set_where(texture::position({ m_position[geometry::AXES::X], m_position[geometry::AXES::Y]}));
	m_sprite->set_rotation(m_rotation);
}