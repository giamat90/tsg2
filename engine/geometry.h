#pragma once

#include "tsg2.h"
#include <tsg/io.h>

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
		inline scalar get_x() const { return m_v[0]; }
		inline scalar get_y() const { return m_v[1]; }
		template <typename = std::enable_if<Dim == 3u, void>>
		inline scalar get_z() const { return m_v[2]; }	
	public:
		void zero() {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] = scalar();
			}
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
		inline vector<Dim>& operator-=(const vector<Dim>& other) {
			for (size_t i = 0u; i < Dim; ++i) {
				m_v[i] -= other.m_v[i];
			}
			return *this;
		}
		// multiply for a scalar
		inline vector<Dim> operator*(const scalar k) {
			vector<Dim> ret;
			for (size_t i = 0u; i < Dim; ++i) {
				ret.m_v[i] = m_v[i] * k;
			}
			return ret;
		}
	private:
		scalar m_v[Dim];
	};

	using point2D = vector<2>;
	using point3D = vector<3>;

	class TSG2_API shape {};

	class TSG2_API rectangle : public shape {
	public:
		rectangle(const scalar x, const scalar y, const scalar w, const scalar h) :
			m_x(x), m_y(y), m_w(w), m_h(h) {};

		// setter
		inline void set_x(const scalar x) { m_x = x; };
		inline void set_y(const scalar y) { m_y = y; };
		inline void set_w(const scalar w) { m_w = w; };
		inline void set_h(const scalar h) { m_h = h; };
		// getters
		inline scalar get_x() const { return m_x; };
		inline scalar get_y() const { return m_y; };
		inline scalar get_w() const { return m_w; };
		inline scalar get_h() const { return m_h; };
	private:
		scalar m_x{};
		scalar m_y{};
		scalar m_w{};
		scalar m_h{};
	};

	class TSG2_API circle : public shape {
	public:
		circle(const scalar x = 0.0f, const scalar y = 0.0f, const scalar r = 0.0f) : m_center(x, y), m_radius(r) {}
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
