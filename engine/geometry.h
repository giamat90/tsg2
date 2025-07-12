#pragma once

#include "tsg2.h"
#include <tsg/io.h>
#include <tsg/math.h>
#include <initializer_list>
#include <cassert>
#include <cmath>
#include <limits>

//#define DOUBLE_PRECISION

namespace geometry {
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
	using scalar = float;
	using degree_t = tsg::degree<scalar>;
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
		inline quaternion operator *=(const quaternion& other)
		{
			m_w = m_w * other.m_w - m_i * other.m_i - m_j * other.m_j - m_k * other.m_k;
			m_i = m_w * other.m_i + m_i * other.m_w + m_j * other.m_k - m_k * other.m_j;
			m_j = m_w * other.m_j + m_j * other.m_w + m_k * other.m_i - m_i * other.m_k;
			m_k = m_w * other.m_k + m_k * other.m_w + m_i * other.m_j - m_j * other.m_i;
			return *this;
		}

		inline quaternion& operator+(const tsg::vector<scalar, 3>& vector)
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
			ELIPSE			
		};
		//virtual void translate(const position) = 0;
		//virtual void rotate(const scalar) = 0;
	/*	virtual position get_center() = 0;
		virtual scalar get_rotation() = 0;*/
	protected:
		TYPE m_type{ TYPE::NONE };

	};

	/*
	*			(A)------|
	*			|  \	 |
	*			|	\	 |
	*			|	 \   |
	*			|	  \  |
	*			|	   \ |
	*			|	    \|
	*			|-------(B)
	*/
	template <size_t Dim>
	class TSG2_API rectangle : public shape {
	public:
		rectangle() : shape(), m_center(), m_half_sizes() {};
		rectangle(const tsg::vector<scalar, Dim>& center, const tsg::vector<scalar, Dim>& half_sizes) : 
			shape(), m_center(center), m_half_sizes(half_sizes) {};

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
		inline scalar get_max(const std::size_t axes) {
			assert(axes < Dim);
			return m_center[axes] + m_half_sizes[axes];
		};
		inline scalar get_min(const std::size_t axes) {
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
		tsg::vector<scalar, Dim> m_center{};
		tsg::vector<scalar, Dim> m_half_sizes{};
		/* Leave only one rappresetation:
		* quaternions less space
		* matrix less computations -> more efficiency at real time.
		*/
		quaternion m_direction{};
		tsg::matrix<scalar, Dim, Dim> m_base{ tsg::matrix<scalar, Dim, Dim>::TYPE::IDENTICAL };
	};

	using box2D = rectangle<2>;
	using box3D = rectangle<3>;

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
}
