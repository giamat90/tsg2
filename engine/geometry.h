#pragma once

#include "tsg2.h"
#include <tsg/io.h>
#include <initializer_list>
#include <cassert>

namespace geometry {
	enum AXES {
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

	enum class PRECISION {
		SINGLE,
		DOUBLE
	};

	using scalar = float;

	class TSG2_API position {
	public:
		position() = default;
		~position() = default;
		virtual inline scalar& operator[](const AXES a) = 0;
		virtual inline const scalar& operator[](const AXES a) const = 0;
	};

	template<std::size_t Dim>
	class vector : public position {
	public:
		vector() : position() {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] = scalar();
			}
		}

		 vector(const scalar k) : position() {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] = k;
			}
		}

		vector(const std::initializer_list<scalar>& list) {
			assert(Dim == list.size());
			auto it = list.begin();
			for (std::size_t i{}; i < Dim, it != list.end(); ++i, ++it) {
				m_v[i] = *it;
			}
		}

		/*
		* It is possible create a vector from another one of minor size
		*/
		template <size_t S, std::enable_if_t<S < Dim, void>>
		explicit vector(const vector<S> other) {
			static_assert(S < Dim);
			for (size_t i{}; i < S; ++i) {
				m_v[i] = other.m_v[i];
			}
			for (size_t j = S; j < Dim; ++j) {
				m_v[j] = scalar{};
			}
		}

		template <typename = std::enable_if<Dim == 2u, void>>
		vector(const scalar x, const scalar y) : position() {
			static_assert(Dim == 2u);
			m_v[AXES::X] = x;
			m_v[AXES::Y] = y;
		}

		template <typename = std::enable_if<Dim == 3u, void>>
		vector(const scalar x, const scalar y, const scalar z) : position() {
			static_assert(Dim == 3u);
			m_v[AXES::X] = x;
			m_v[AXES::Y] = y;
			m_v[AXES::Z] = z;
		}

		vector(const vector<Dim>& other) {
			for (std::size_t i = 0u; i < Dim; ++i) {
				m_v[i] = other.m_v[i];
			}
		}
	public:
		// getters
		inline scalar get_x() const { return m_v[0]; }
		inline scalar get_y() const { return m_v[1]; }
		template <typename = std::enable_if<Dim == 3u, void>>
		inline scalar get_z() const { return m_v[2]; }	
	public:
		/* Special cases */
		void zero() {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] = scalar();
			}
		}
		bool is_zero() const {
			for (size_t i = 0u; i < Dim; ++i) {
				if (m_v[i] > scalar() || m_v[i] < scalar()) {
					return false;
				}
			}
			return true;
		}
		void one() {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] = scalar(1);
			}
		}
		bool is_one() const {
			auto acc = scalar(1);
			for (size_t i = 0u; i < Dim; ++i) {
				if ((acc *= m_v[i]) != scalar(1)) {
					return false;
				}
			}
			return true;
		}
	public:
		/* Operator overloading */
		// operator[]
		inline scalar& operator[](const AXES a) override {
			if (a < Dim) {
				return m_v[a];
			}
			else {
				tsg::print("Axes not valid for this vector");
				throw(std::numeric_limits<scalar>::infinity());
			}
		}
		inline const scalar& operator[](const AXES a) const override {
			if (a < Dim) {
				return m_v[a];
			}
			else {
				tsg::print("Axes not valid for this vector");
				throw(std::numeric_limits<scalar>::infinity());
			}
		}
		// sum between vectors
		inline vector<Dim>& operator+=(const vector<Dim>& other) {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] += other.m_v[i];
			}
			return *this;
		}		
		inline friend vector<Dim> operator+(vector<Dim> lhs, const vector<Dim>& rhs) {
			lhs += rhs;
			return lhs;
		}
		inline vector<Dim>& operator-=(const vector<Dim>& other) {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] -= other.m_v[i];
			}
			return *this;
		}
		inline friend vector<Dim> operator-(vector<Dim> lhs, const vector<Dim>& rhs) {
			lhs -= rhs;
			return lhs;
		}
		// multiply for a scalar
		/* for k * vec */
		inline vector<Dim> operator*(const scalar k) {
			vector<Dim> ret(scalar(1));
			for (size_t i = 0u; i < Dim; ++i) {
				ret.m_v[i] = m_v[i] * k;
			}
			return ret;
		}
		/* for vec * k */
		inline friend vector<Dim> operator*(const scalar k, vector<Dim> vec) {
			return  vec.operator*(k);
		};
	private:
		scalar m_v[Dim];
	};

	using point2D = vector<2>;
	using point3D = vector<3>;
	using vector2D = vector<2>;
	using vector3D = vector<3>;

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
		rectangle() : shape(), m_center(), m_a(), m_b(), m_width(), m_height(), m_depth() {
			static_assert(Dim <= 3);
			m_type = TYPE::RECTANGLE;
		}

		rectangle(const vector<Dim>& top_left, const vector<Dim>& bottom_right) : shape(),
			m_a(top_left), m_b(bottom_right)
		{
			static_assert(Dim <= 3);
			m_width = std::fabsf(top_left.get_x() - bottom_right.get_x());
			m_height = std::fabsf(top_left.get_y() - bottom_right.get_y());
			m_depth = std::fabsf(top_left.get_z() - bottom_right.get_z());

			m_center = (top_left + bottom_right) * 0.5f;
		}

		// setter
		inline void set_center(const vector<Dim>& c) { 
			m_center = c; 
			m_a = { m_center.get_x() + m_width * 0.5f, m_center.get_y() + m_height * 0.5f, m_center.get_z() + m_depth * 0.5f };
			m_b = { m_center.get_x() - m_width * 0.5f, m_center.get_y() - m_height * 0.5f, m_center.get_z() - m_depth * 0.5f};
		}
		inline void set_top(const vector<Dim>& x) { m_a = x; };
		inline void set_bottom(const vector<Dim>& y) { m_b = y; };
		// getters
		inline vector<Dim> get_top() const { return m_a; };
		inline vector<Dim> get_bottom() const { return m_b; };
		inline vector<Dim> get_center() const { return m_center; };
		inline scalar get_width() const { return m_width; };
		inline scalar get_height() const { return m_height; };
		inline scalar get_depth() const { return m_depth; };
		inline scalar get_max(const AXES axes) {
			return std::max(m_a[axes], m_b[axes]);
		};
		inline scalar get_min(const AXES axes) {
			return std::min(m_a[axes], m_b[axes]);
		}
	public:
		/* tronsform methods */
		void translate(const vector<Dim>& pos) {
			m_center += pos;
			m_a += pos;
			m_b += pos;
		}
		void rotate(const scalar angle) {
			m_a = 
			{ 
				m_a[geometry::AXES::X] * cos(angle) - m_a[geometry::AXES::Y] * sin(angle),
				m_a[geometry::AXES::X] * cos(angle) + m_a[geometry::AXES::Y] * sin(angle), 
				scalar(0)
			};
			m_b =
			{
				m_b[geometry::AXES::X] * cos(angle) - m_b[geometry::AXES::Y] * sin(angle),
				m_b[geometry::AXES::X] * cos(angle) + m_b[geometry::AXES::Y] * sin(angle),
				scalar(0)
			};
		}
	private:
		vector<Dim> m_center{};
		vector<Dim> m_a{};
		vector<Dim> m_b{};
		scalar m_width{};	// width
		scalar m_height{};	// height
		scalar m_depth{};	// depth
	};

	using box2D = rectangle<2>;
	using box3D = rectangle<3>;

	class TSG2_API circle : public shape {
	public:
		circle(const scalar x = 0.0f, const scalar y = 0.0f, const scalar r = 0.0f) : m_center(x, y), m_radius(r) {}
	public:
		void translate(const position&) {};
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
