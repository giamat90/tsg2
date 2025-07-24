#include "geometry.h"
#include <tsg/logger.h>

namespace geometry {
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
