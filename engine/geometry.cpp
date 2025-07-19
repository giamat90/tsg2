#include "geometry.h"
#include <tsg/logger.h>

namespace geometry {
	/*
	* Distance between two point in 3D space
	*/
	scalar distance(const point3D& p, const point3D& q) {
		return (p - q).get_norm();
	}
	/*
	* Distance between a point and a plane in 3D space
	*/
	scalar distance(const point3D& p, const finite_plane& f) {
		return  vector3D::dot((p - f.m_center), f.m_normal);
	}
	/*
	* Distance between a point and a segment in 3D space
	*/
	scalar distance(const point3D& p, segment& s) {
		assert(0);
		return scalar(0);
	}
	/*
	* segment implementation
	*/
	point3D segment::closest_to(const point3D& p) {
		vector3D segment_vector = (m_vector * m_lenght);
		scalar t = vector3D::dot(segment_vector, (p - m_start));
		t = std::max(scalar(0), std::min(scalar(1), t));
		return m_start + (segment_vector * t);
	};
	/*
	* Explaination:
	* segment this:		p(s) = m_center + s * da		s \in [0,1]
	* segment other:	q(t) = other.m_center + t * db	t \in [0,1]
	* The result is given to the minimum of this function:
	* d^2(s,t) = |p(s) - q(t)|^2 = 
	*		= |m_center + s * da - other.m_center - t * db|^2
	*		= |r + s * da - t * db |^2
	* with r = m_center - other.m_center
	* => d^2(s,t) = dot( (r + s * da - t * db), (r + s * da - t * db) ) = 
	*		= dot(r,r) + 2s*dot(da,r) - 2t*dot(db,r) + s^2*dot(da,da) - 2st*dot(da,db) + t^2*dot(db,db)
	*		= s^2 * da2 - 2st * dab + t^2 * db2 + 2s * dot(da,r) - 2t * dot(db,r) + r2
	* The minimus is given by the solution of the sistem of partial derivation:
	*	df / ds = 0		=>		2s * da2 - 2t * dab 2 * dot(da,r)
	*	df / dt = 0		=>		-2s * dab + 2t * db2 -2 * dot(db,r)
	*	=>	s*da2 - t*dab = -dot(da,r)
	*	=>	-s*dab + tdb2 = dot(db,r)
	* In matrix form:
	*	|  da2	-dab | | s |   | -dot(da,r) |
	*	|			 | |   | = |			|
	*	| -dab	 db2 | | t |   |  dot(db,r) |
	*/
	std::pair<point3D, point3D> segment::closest_to(const segment& other) {
		vector3D da = m_end - m_start;				// direction of segment a
		vector3D db = other.m_end - other.m_start;	// direction of segment b
		vector3D r = m_start - other.m_start;		// 

		scalar da2 = vector3D::dot(da, da);	// |da|^2
		scalar db2 = vector3D::dot(db, db); // |db|^2
		scalar dab = vector3D::dot(da, db); // parallelism
		scalar da_over_r = vector3D::dot(da, r);	// projection r over va
		scalar db_over_r = vector3D::dot(db, r);	// projection r over vb

		/* scalar parameters of the segment parametric equation  */
		scalar s{};
		scalar t{};
		/* checking for almost parallel edges */
		scalar denom = da2 * db2 - dab * dab;	// determinant of the cramer system
		if (denom < scalar_zero) {
			t = dab > da2 ? da_over_r / dab : db_over_r / db2;
		}
		else {
			s = (dab * db_over_r - db2 * da_over_r) / denom;
			t = (da2 * db_over_r - dab * da_over_r) / denom;
		}

		return { 
			m_start + da * std::max(scalar(0), std::min(scalar(1), s)), 
			other.m_start + db * std::max(scalar(0), std::min(scalar(1), t))
		};
	}

	/*
	* obb_contact implementation
	*/	

	std::expected<obb_contact::contact, bool> obb_contact::get_contact() {
		if (m_contacts.size() > 0) {
			return *m_contacts.begin();
		}
		else {
			return std::unexpected(false);
		}
	}

	void obb_contact::compute() {
		vertex_face();
		edge_edge();
		/* compute best contact based on the greater penetration */
		std::sort(m_contacts.begin(), m_contacts.end(), [](const contact& c1, const contact& c2) { return c1.m_penetration > c2.m_penetration; });
#if _DEBUG

		tsg::logger::get_instance().write("Number of contacts: {}", m_contacts.size());
		for (auto it = m_contacts.begin(); it != m_contacts.end(); ++it) {
			tsg::logger::get_instance().write(
				tsg::string("contact type {}", (it->m_type == TYPE::EDGE_EDGE) ? "EDGE_EDGE" : ((it->m_type == TYPE::VERTEX_FACE) ? "VERTEX_FACE" : "UNKNOWN")) +
				tsg::string("contact pen  {}", it->m_penetration) +
				tsg::string("normal ({},{},{})", it->m_normal.get<AXES::X>(), it->m_normal.get<AXES::Y>(), it->m_normal.get<AXES::Z>()) +
				tsg::string("point  ({},{},{})", it->m_point.get<AXES::X>(), it->m_point.get<AXES::Y>(), it->m_point.get<AXES::Z>())
			);
		}
#endif
	}

	void obb_contact::vertex_face() {
		/* 
		* Verteces agains faces 
		*/
		auto lambda_vertex_against_face = [](box3D::vertexes v, box3D::faces f, std::vector<contact>& contacts, bool swap) {
			for (auto vertex_it = v.begin(); vertex_it != v.end(); ++vertex_it) {
				for (auto face_it = f.begin(); face_it != f.end(); ++face_it) {
					/* check if the vertex is penetrating the face */
					if (scalar distance = geometry::distance(*vertex_it, *face_it) < scalar(0)) {
						point3D projected_vertex = face_it->project(*vertex_it);
						if (face_it->contain(projected_vertex)) {
							contacts.emplace_back(projected_vertex, swap ? face_it->get_normal() * scalar(-1) : face_it->get_normal(), geometry::abs(distance), TYPE::VERTEX_FACE);
						}
					}

				}
			}
		};
		/* checking obb1 vertexes against obb2 faces */
		lambda_vertex_against_face(m_obb1.get_vertexes(), m_obb2.get_faces(), m_contacts, false);
		/* checking obb2 vertexes against obb1 faces */
		lambda_vertex_against_face(m_obb2.get_vertexes(), m_obb1.get_faces(), m_contacts, true);
	}

	void obb_contact::edge_edge() {
		box3D::edges edges1 = m_obb1.get_edges();
		box3D::edges edges2 = m_obb2.get_edges();
		for (auto edg1_it = edges1.begin(); edg1_it != edges1.end(); ++edg1_it) {
			for (auto edg2_it = edges2.begin(); edg2_it != edges2.end(); ++edg2_it) {
				/* Skip computation for almost parallel edges */
				constexpr scalar parallel_threshold{ 0.95 };
				if (vector3D::dot(edg1_it->get_direction(), edg2_it->get_direction()) > parallel_threshold) {
					continue;
				}
				else {
					auto [point_edg1, point_edg2] = edg1_it->closest_to(*edg2_it);
					scalar d = distance(point_edg1, point_edg2);
					/* Normal have to points from obb1 to obb2 */
					if (d < scalar_zero) {
						vector3D normal = vector3D::cross(edg1_it->get_direction(), edg2_it->get_direction());
						vector3D center2center = m_obb2.get_center() - m_obb1.get_center();
						if (vector3D::dot(normal, center2center) < scalar(0)) {
							normal *= scalar(- 1);
						}
						scalar penetration{};
						/* Compute edge-edge penetration */
						{
							point3D center_edge1 = edg1_it->get_direction() * edg1_it->get_lenght() * scalar(0.5f);
							point3D center_edge2 = edg2_it->get_direction() * edg2_it->get_lenght() * scalar(0.5f);
							if (m_obb1.contains(center_edge2) || m_obb2.contains(center_edge1)) {
								penetration = (point_edg1 - point_edg2).get_norm();
							}
						}
						if (penetration > scalar(0)) {
							m_contacts.emplace_back(scalar(0.5) * (point_edg1 + point_edg2), normal, penetration, TYPE::EDGE_EDGE);
						}
					}
				}
			}
		}
	}
}
