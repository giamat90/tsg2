#pragma once

#include "tsg2.h"
#include "io/io.h"

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

	class TSG2_API position {
	public:
		position() = default;
		~position() = default;
		virtual inline float& operator[](const AXES a) = 0;
		virtual inline const float& operator[](const AXES a) const = 0;
	};

	template<std::size_t Dim>
	class vector : public position {
	public:
		vector() : position() {}
		vector(const float x, const float y) : position() {
			static_assert(Dim == 2u);
			m_v[AXES::X] = x;
			m_v[AXES::Y] = y;
		}
		vector(const float x, const float y, const float z) : position() {
			static_assert(Dim == 3u);
			m_v[AXES::X] = x;
			m_v[AXES::Y] = y;
			m_v[AXES::Z] = z;
		}
		inline float get_x() const { return m_v[0]; }
		inline float get_y() const { return m_v[1]; }
		/* Only for 3d vectors */
		template <typename = std::enable_if<Dim == 3u, void>>
		inline float get_z() const { return m_v[2]; }	// only for 3d vectors
		// operator[]
		inline float& operator[](const AXES a) override {
			if (a < Dim) {
				return m_v[a];
			}
			else {
				tsg::print("Axes not valid for this vector");
				throw(std::numeric_limits<float>::infinity());
			}
		}
		inline const float& operator[](const AXES a) const override {
			if (a < Dim) {
				return m_v[a];
			}
			else {
				tsg::print("Axes not valid for this vector");
				throw(std::numeric_limits<float>::infinity());
			}
		}
	private:
		float m_v[Dim];
	};

	class TSG2_API shape {};

	class TSG2_API rectangle : public shape {
	public:
		rectangle(const float x, const float y, const float w, const float h) :
			m_x(x), m_y(y), m_w(w), m_h(h) {};

		// setter
		inline void set_x(const float x) { m_x = x; };
		inline void set_y(const float y) { m_y = y; };
		inline void set_w(const float w) { m_w = w; };
		inline void set_h(const float h) { m_h = h; };
		// getters
		inline float get_x() const { return m_x; };
		inline float get_y() const { return m_y; };
		inline float get_w() const { return m_w; };
		inline float get_h() const { return m_h; };
	private:
		float m_x{};
		float m_y{};
		float m_w{};
		float m_h{};
	};

	class TSG2_API circle : public shape {
	public:
		circle(const float x, const float y) : m_x(x), m_y(y) {}
	private:
		float m_x;
		float m_y;
	};


	class TSG2_API surface : public shape {
	public:
		surface() = default;
		~surface() = default;
		inline unsigned& operator[](MEASURE m) {
			if (MEASURE::DEPTH != m) {
				return m_measure[m];
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
