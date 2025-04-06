#pragma once

#include "tsg2.h"
#include "geometry.h"
#include <tsg/types.h>

using geometry::surface;

class TSG2_API texture {
public:
	using position = geometry::vector<2>;
	using angle = float;
	using scale = float;
public:
	texture(int w = 0, int h = 0, const std::string& asset = "") : m_width(w), m_height(h) { load(asset); };
	virtual ~texture() { unload(); };
	virtual void load(const std::string& asset = "") {};
	virtual void unload() {};
public:
	inline void set_size(int w = 0, int h = 0) { m_width = w; m_height = h; }
	inline void set_scale(const scale& s) { m_scale = s; }
	inline void set_where(const position& p) { m_position = p; }
	inline void set_rotation(const angle a) { m_angle = a; }
	inline scale get_scale() { return m_scale; }
	inline position get_where() { return m_position; }
	inline angle get_rotation() { return m_angle; }
public: // factory-method
	static texture* create_texture();
protected:
	scale m_scale{};
	position m_position{};
	angle m_angle{};
	int m_width{};
	int m_height{};
};