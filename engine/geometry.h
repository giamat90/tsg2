#pragma once

#include "tsg2.h"
#include <tsg/io.h>
#include <tsg/math.h>
#include <tsg/logger.h>
#include <initializer_list>
#include <cassert>
#include <cmath>
#include <limits>
#include <concepts>

//#define DOUBLE_PRECISION

namespace geometry {
	template <std::size_t Dim>
	concept Dimension2D = Dim == 2;

	template <std::size_t Dim>
	concept Dimension3D = Dim == 3;

	enum AXES : std::size_t {
		X = 0u,
		Y = 1u,
		Z = 2u
	};

	enum MEASURE {
		WIDTH = 0,
		HEIGHT = 1,
		DEPTH = 2
	};

	enum class SHAPE_TYPE {
		NONE,
		RECTANGLE,
		CIRCLE,
		TRIANGLE,
		SPHERE,
		PYRAMID
	};

#ifdef DOUBLE_PRECISION
	using scalar = double;
	using degree_t = tsg::degree<scalar>;
	scalar inline abs(const scalar v) {
		return std::abs(v);
	}
	scalar inline sqrt(const scalar v) {
		return std::sqrt(v);
	}
	scalar inline arccos(const scalar v) {
		return std::acos(v);
	}
	scalar inline cos(const scalar v) {
		return std::cos(v);
	}
	scalar inline sin(const scalar v) {
		return std::sin(v);
	}
	constexpr scalar epsilon = std::numeric_limits<double>::epsilon();
#else
#define SINGLE_PRECISION
	using scalar = float;
	using degree_t = tsg::degree<scalar>;
	scalar inline abs(const scalar v) {
		return std::fabsf(v);
	}
	scalar inline sqrt(const scalar v) {
		return std::sqrtf(v);
	}
	scalar inline arccos(const scalar v) {
		return std::acosf(v);
	}
	scalar inline arcsin(const scalar v) {
		return std::asinf(v);
	}
	scalar inline cos(const scalar v) {
		return std::cosf(v);
	}
	scalar inline sin(const scalar v) {
		return std::sinf(v);
	}
	constexpr scalar epsilon = std::numeric_limits<float>::epsilon();
#endif

	using point2D = tsg::vector<scalar, 2>;
	using point3D = tsg::vector<scalar, 3>;
	using vector2D = tsg::vector<scalar, 2>;
	using vector3D = tsg::vector<scalar, 3>;
	using matrix3D = tsg::matrix<scalar, 3, 3>;
	/*
	* Quaternion class for orientation rappresentation
	*/
	class quaternion {
	public:
		quaternion(const scalar w = scalar(1), const scalar i = scalar(0), const scalar j = scalar(0), const scalar k = scalar(0)) :
			m_w(w), m_i(i), m_j(j), m_k(k) {};
		quaternion(const tsg::vector<scalar, 3>& vec) :
			m_w(scalar(0)), m_i(vec[AXES::X]), m_j(vec[AXES::Y]), m_k(vec[AXES::Z]) {};
		inline void normalize() {
			scalar d = m_w * m_w + m_i * m_i + m_j * m_j + m_k * m_k;

			// Check for zero length quaternion, and use the no-rotation
			// quaternion in that case.
			if (d < epsilon) {
				m_w = 1;
			}
			else {
				d = scalar(1) / sqrt(d);  //((real)1.0) / real_sqrt(d);
				m_w *= d;
				m_i *= d;
				m_j *= d;
				m_k *= d;
			}
		}
		inline quaternion& operator *=(const quaternion& other)
		{
			m_w = m_w * other.m_w - m_i * other.m_i - m_j * other.m_j - m_k * other.m_k;
			m_i = m_w * other.m_i + m_i * other.m_w + m_j * other.m_k - m_k * other.m_j;
			m_j = m_w * other.m_j + m_j * other.m_w + m_k * other.m_i - m_i * other.m_k;
			m_k = m_w * other.m_k + m_k * other.m_w + m_i * other.m_j - m_j * other.m_i;
			return *this;
		}

		inline friend quaternion& operator*(quaternion& lhs, const quaternion& rhs) {
			return lhs *= rhs;
		}

		inline quaternion& operator+=(const quaternion& other)
		{
			quaternion retVal;
			/*
			// Vector component is:
			// ps * qv + qs * pv + pv x qv
			Vector3 qv(q.x, q.y, q.z);
			Vector3 pv(p.x, p.y, p.z);
			Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
			retVal.x = newVec.x;
			retVal.y = newVec.y;
			retVal.z = newVec.z;

			// Scalar component is:
			// ps * qs - pv . qv
			retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

			return retVal;
			*/

			// Vector component is:
			// ps * qv + qs * pv + pv x qv
			vector3D pv({m_i, m_j, m_k });
			vector3D qv({ other.m_i, other.m_j, other.m_k });
			vector3D newVec = m_w * qv + other.m_w * pv + vector3D::cross(pv, qv);
			this->m_i = newVec[AXES::X];
			this->m_j = newVec[AXES::Y];
			this->m_k = newVec[AXES::Z];

			// Scalar component is:
			// ps * qs - pv . qv
			this->m_w = m_w * other.m_w - vector3D::dot(pv, qv);

			return *this;
		}

		inline friend quaternion& operator+(quaternion& lhs, const quaternion& rhs)
		{
			return lhs += rhs;
		}

		inline quaternion& operator+=(const tsg::vector<scalar, 3>& vector)
		{
			quaternion q(vector);
			q *= *this;
			m_w += q.m_w * scalar(0.5);
			m_i += q.m_i * scalar(0.5);
			m_j += q.m_j * scalar(0.5);
			m_k += q.m_k * scalar(0.5);
			return *this;
		}
		inline quaternion& rotate(const tsg::vector<scalar, 3>& vector) {
			(*this) *= quaternion(vector);
			return *this;
		}
	private:
		scalar m_w{};
		scalar m_i{};
		scalar m_j{};
		scalar m_k{};
	};

	class TSG2_API shape {
	public:
		enum class TYPE {
			NONE,
			RECTANGLE,
			SPHERE,
			CIRCLE,
			ELIPSE,
			PLANE
		};
	protected:
		TYPE m_type{ TYPE::NONE };
	};
	/*
	* forward declarations for friend function
	*/
	class finite_plane;
	class segment;
	scalar distance(const point3D& p, const finite_plane& f);
	scalar distance(const point3D& p, const segment& s);
	/*
	* class to compute a finite line in 3D space
	*/
	class segment {
	public:
		segment() = default;
		virtual ~segment() = default;
		segment(const point3D& start, const point3D& end) : 
			m_start(start), m_end(end), m_vector((end - start).get_normalized()), m_lenght((end - start).get_norm()) {};
		segment(const segment& other) :
			m_start(other.m_start), m_end(other.m_end), m_lenght(other.m_lenght), m_vector(other.m_vector) {
		};
	public:
		segment& operator=(const segment& other) {
			this->m_end = other.m_end;
			this->m_start = other.m_start;
			this->m_lenght = other.m_lenght;
			this->m_vector = other.m_vector;
			return *this;
		};
	public:
		friend scalar distance(const point3D&, const segment&);
	private:
		point3D m_start;
		point3D m_end;
		vector3D m_vector{};
		scalar m_lenght;
	};
	/*
	* class to compute a finite/limited plane, like a face or a wall, in 3D space
	*/
	class finite_plane {
	public:
		finite_plane() = default;
		finite_plane(const point3D& c) : m_center(c) {}
		virtual ~finite_plane() = default;
	public:
		inline vector3D project(const point3D& p) {
			return p - m_normal * vector3D::dot(m_normal, p - m_center);
		}
		inline bool contain(const point3D& p) {
			vector3D projection{ p - m_center };
			scalar u{ vector3D::dot(projection, m_base[1])};
			scalar v{ vector3D::dot(projection, m_base[2])};
			return geometry::abs(u) <= m_half_sizes[0] && geometry::abs(v) <= m_half_sizes[1];
		}
		friend scalar distance(const point3D&, const finite_plane&);
	protected:
		point3D m_center;
		vector3D m_normal;
		vector3D m_base[2];
		vector2D m_half_sizes;
	};

	/*
	* The box has validity only for 2 or 3 dimension (as per now). 
	* Then the concept.
	*/
	template <std::size_t Dim>
	concept BoxDimension = Dimension2D<Dim> || Dimension3D<Dim>;
	// impl
	template <std::size_t Dim> requires BoxDimension<Dim>
	class TSG2_API box : public shape {
	public:
		box() : shape(), m_center(), m_half_sizes() {};
		box(const tsg::vector<scalar, Dim>& center, const tsg::vector<scalar, Dim>& half_sizes) :
			shape(), m_center(center), m_half_sizes(half_sizes) 
		{
			/* 
			* Compute vertexes: center ± sizes computed with bit operations, very efficent way
			*   i | Binario | Bit2 | Bit1 | Bit0 | Z    | Y    | X
			*	--|---------|------|------|------|------|------|------
			*	0 |   000   |  0   |  0   |  0   | -ext | -ext | -ext
			*	1 |   001   |  0   |  0   |  1   | -ext | -ext | +ext
			*	2 |   010   |  0   |  1   |  0   | -ext | +ext | -ext
			*	3 |   011   |  0   |  1   |  1   | -ext | +ext | +ext
			*	4 |   100   |  1   |  0   |  0   | +ext | -ext | -ext
			*	5 |   101   |  1   |  0   |  1   | +ext | -ext | +ext
			*	6 |   110   |  1   |  1   |  0   | +ext | +ext | -ext
			*	7 |   111   |  1   |  1   |  1   | +ext | +ext | +ext
			*  The order is this:
			*      6----------7
			*     /|         /|
			*    / |        / |
			*   2----------3  |
			*   |  |       |  |
			*   |  4-------|--5
			*   | /        | /
			*   |/         |/
			*   0----------1
			*/
			// TODO: verify rows or columns.
			point3D vertex = m_center;
			for (std::size_t i = 0; i < 8; ++i) {
				/*
				m_vertexes[i] = {
					m_center + m_base.get_row<0>() * ((i & 1) ? m_half_sizes[0] : -m_half_sizes[0]),
					m_center + m_base.get_row<1>() * ((i & 1) ? m_half_sizes[1] : -m_half_sizes[1]),
					m_center + m_base.get_row<2>() * ((i & 1) ? m_half_sizes[2] : -m_half_sizes[2])
				};
				*/
				m_vertexes[i] = m_center +
					m_base.get_row<0>() * ((i & 1) ? m_half_sizes[0] : -m_half_sizes[0]) +
					m_base.get_row<1>() * ((i & 1) ? m_half_sizes[1] : -m_half_sizes[1]) +
					m_base.get_row<2>() * ((i & 1) ? m_half_sizes[2] : -m_half_sizes[2]);
			}
			/*
			* Compute edges:
			* Given the vertexe's order (see above), the indexes that determine every edges are the following 12:
			*/
			std::size_t vertexes_index[12][2] = {
				{0,1}, {1, 3}, {3, 2}, {2, 0},	// front face's ages
				{4,5}, {5,7}, {7,6}, {6,4},		// behind face's edges
				{1,5}, {0,4}, {3,7}, {2,6}		// horizontal edges
			};
			/* With these vertex index we can compute every edges as edge(end - start). */
			for (std::size_t i = 0u; i < 12u; ++i) {
				m_edges[i] = segment(m_vertexes[vertexes_index[i][0]], m_vertexes[vertexes_index[i][1]]);
			}
			/*
			* Compute faces
			*/
			for (std::size_t i = 0u; i < 6u; ++i) {
				m_faces[i] = finite_plane(m_center);
			}
			/*
			std::vector<Face> faces;        
			// Create 6 faces (±X, ±Y, ±Z in local space)
			for (int axis = 0; axis < 3; axis++) {
				for (int sign = -1; sign <= 1; sign += 2) {
					Face face;
					face.normal = u[axis] * (float)sign;
					face.center = center + face.normal * extents.x; // Will be corrected below
                
					// Correct center position and set extents
					if (axis == 0) {
						face.center = center + u[0] * (extents.x * sign);
						face.tangent1 = u[1];
						face.tangent2 = u[2];
						face.extents2D = Vec3(extents.y, extents.z, 0);
					} else if (axis == 1) {
						face.center = center + u[1] * (extents.y * sign);
						face.tangent1 = u[0];
						face.tangent2 = u[2];
						face.extents2D = Vec3(extents.x, extents.z, 0);
					} else {
						face.center = center + u[2] * (extents.z * sign);
						face.tangent1 = u[0];
						face.tangent2 = u[1];
						face.extents2D = Vec3(extents.x, extents.y, 0);
					}                
					faces.push_back(face);
				}
			}        
			return faces;
			*/
		};
		box(const box<Dim>& other) : shape(),
			m_center(other.m_center),
			m_half_sizes(other.m_half_sizes),
			m_direction(other.m_direction),
			m_base(other.m_base) 
		{
			for (std::size_t i = 0u; i < 8; ++i) {
				m_vertexes[i] = other.m_vertexes[i];
			}
			for (std::size_t i = 0u; i < 12; ++i) {
				m_edges[i] = other.m_edges[i];
			}
			for (std::size_t i = 0u; i < 6; ++i) {
				m_faces[i] = other.m_faces[i];
			}
		};
		virtual ~box() {};


		// setter
		inline void set_center(const tsg::vector<scalar, Dim>& c) { m_center = c; }
		// getters
		inline tsg::vector<scalar, Dim> get_center() const { return m_center; };
		template <std::size_t Ax>
		inline tsg::vector<scalar, Dim> get_max_point() const {
			static_assert(Ax < Dim);
			return m_center[Ax] + m_half_sizes[Ax];
		}
		template <std::size_t Ax>
		inline scalar get_size() const {
			static_assert(Ax < Dim);
			return scalar(2)*m_half_sizes[Ax];
		}
		template <std::size_t Ax>
		inline scalar get_half_size() const {
			static_assert(Ax < Dim);
			return m_half_sizes[Ax];
		}
		inline tsg::vector<scalar, Dim> get_sizes() const {
			return scalar(2) * m_half_sizes;
		}
		inline tsg::vector<scalar, Dim> get_half_sizes() const {
			return m_half_sizes;
		}
		inline scalar get_max(const std::size_t axes) const {
			assert(axes < Dim);
			return m_center[axes] + m_half_sizes[axes];
		};
		inline scalar get_min(const std::size_t axes) const {
			assert(axes < Dim);
			return m_center[axes] - m_half_sizes[axes];
		};
		inline tsg::matrix<scalar, Dim, Dim>& get_axes() { return m_base; };
	public:
		/* tronsform methods */
		void translate(const tsg::vector<scalar, Dim>& pos) {
			m_center += pos;
		}
		void rotate(const scalar angle) {
			/* TODO */
		}
	private:
		/*
		* Quantities necessaries for creation
		*/
		point3D m_center{};
		vector3D m_half_sizes{};
		/*
		* Derived quantities
		*/
		point3D m_vertexes[8];
		finite_plane m_faces[6];
		segment m_edges[12];
		/* TODO: Leave only one rappresetation:
		* quaternions less space
		* matrix less computations -> more efficiency at real time.
		* Orientation quantities
		*/
		quaternion m_direction{};
		tsg::matrix<scalar, Dim, Dim> m_base{ tsg::matrix<scalar, Dim, Dim>::TYPE::IDENTITY };
	};

	using box2D = box<2>;
	using box3D = box<3>;

	class TSG2_API circle : public shape {
	public:
		circle(const scalar x = 0.0f, const scalar y = 0.0f, const scalar r = 0.0f) : m_center({ x, y }), m_radius(r) {}
	public:
		void translate(const vector2D&) {};
		void rotate(const scalar) {};
	private:
		point2D m_center;
		scalar m_radius;
	};

	class TSG2_API surface : public shape {
	public:
		surface() = default;
		~surface() = default;
		inline unsigned& operator[](MEASURE m) {
			if (MEASURE::DEPTH != m) {
				return m_measure[m];
			}
			else {
				throw;
			}
		}
		inline const unsigned& operator[](MEASURE m) const {
			if (MEASURE::DEPTH != m) {
				return m_measure[m];
			}
			else {
				throw;
			}
		}

	private:
		unsigned m_measure[2];
	};

	class inertia_tensor {
	public:
		enum class TYPE {
			CUBE
		};
		static inline matrix3D get_inverse(const TYPE t) {
			if (TYPE::CUBE == t) {
				return matrix3D(matrix3D::TYPE::DIAGONAL, scalar(1) / scalar(12));
			}
			else {
				tsg::logger::get_instance().write("Wrong inertia tensor type inserted");
				assert(false);
			}
		}

	};

	class obb_contact {
	public:
		obb_contact(geometry::box3D& obb1, geometry::box3D& obb2) : m_obb1(obb1), m_obb2(obb2) {}
		enum class TYPE {
			VERTEX_FACE,
			EDGE_EDGE
		};
		void compute(const TYPE t);
	private:
		void vertex_face();
		void edge_edge();
	private:
		geometry::box3D& m_obb1;
		geometry::box3D& m_obb2;
	};

}
