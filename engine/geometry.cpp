#include "geometry.h"
#include <tsg/logger.h>

namespace geometry {
	scalar distance(const point3D&, const finite_plane& f) {
		/* TODO */
		return scalar(0);
	}

	scalar distance(const point3D&, const segment& s) {
		/* TODO */
		return scalar(0);
	}

	void obb_contact::compute(const TYPE t) {
		switch (t) {
		case(TYPE::VERTEX_FACE):
			vertex_face();
			break;
		case(TYPE::EDGE_EDGE):
			edge_edge();
			break;
		default:
			tsg::logger::get_instance().write("{} unknown type of contact", __FILE__);
			assert(false);
			break;
		}
	}

	void obb_contact::vertex_face() {
		/* 
		* Verteces agains faces 
		*/
		auto vertices = m_obb1;
		auto faces = m_obb1;

	}

	void obb_contact::edge_edge() {
		/* TODO */
	}
}
