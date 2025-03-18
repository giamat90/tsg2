#pragma once

#include "tsg2.h"
#include "geometry.h"
#include <tsg/types.h>

using geometry::surface;

class TSG2_API texture {
public:
	using texture_position = geometry::vector<2>;
public:
	texture(int w = 0, int h = 0, const std::string& asset = "") : m_width(w), m_height(h) { load(asset); };
	virtual ~texture() { unload(); };
	virtual void load(const std::string& asset = "") {};
	virtual void unload() {};
public:
	inline void set_where(const texture_position& p) { m_position = p; }
	inline void set_size(int w = 0, int h = 0) { m_width = w; m_height = h; }
	inline texture_position get_where() { return m_position; }
public: // factory-method
	static texture* create_texture();
protected:
	texture_position m_position{};
	int m_width{};
	int m_height{};
};