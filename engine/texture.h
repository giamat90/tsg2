#pragma once

#include "tsg2.h"
#include "geometry.h"
#include <tsg/types.h>

using geometry::surface;

class TSG2_API texture {
public:
	using texture_position = geometry::vector<2>;
public:
	texture() = default;
	virtual ~texture() {};
public:
	inline void set_where(const texture_position& p) { m_position = p; }
	inline texture_position get_where() { return m_position; }
public: // factory-method
	static texture* create_texture();
private:
	texture_position m_position{};
	int m_width{};
	int m_height{};
};