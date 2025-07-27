#pragma once

#include "tsg2.h"
#include "sprite.h"
#include "mesh.h"
#include "geometry.h"


class TSG2_API game_object {
public:
	game_object();
	virtual ~game_object();
	virtual void init() = 0;
};

class TSG2_API drawable : public game_object {
public:
	drawable();
	virtual ~drawable();
	void print_bounding_volume(const bool print = false) { m_has_bounding_volume = print; }
	bool has_bounding_volume() const { return m_has_bounding_volume; }
protected:
	bool m_has_bounding_volume{ false };
};

class TSG2_API texture_object : public drawable {
public:
	texture_object() = default;
	virtual ~texture_object() = default;
public:
	texture* get_texture() { return m_texture; }
	void set_texture(texture* r) { m_texture = r; }
protected:
	texture* m_texture{ nullptr };
};

class TSG2_API sprite_object : public drawable {
public:
	sprite_object() = default;
	virtual ~sprite_object() = default;
public:
	sprite* get_sprite() { return m_sprite; }
	void set_sprite(sprite* s) { m_sprite = s; }
protected:
	sprite* m_sprite{ nullptr };
};

class TSG2_API mesh_object : public drawable {
public:
	mesh_object() = default;
	virtual ~mesh_object() = default;
public:
	mesh* get_mesh() { return m_mesh; }
	void set_mesh(mesh* m) { m_mesh = m; }
protected:
	mesh* m_mesh{ nullptr };
};

class TSG2_API updateable : public game_object {
public:
	updateable();
	virtual ~updateable();
	virtual void update(const geometry::scalar delta_time) = 0;
};

class TSG2_API static_object : public drawable {
public:
	static_object();
	virtual ~static_object();
};

class TSG2_API dynamic_object : public updateable, public drawable {
public:
	dynamic_object();
	virtual ~dynamic_object();
};

class TSG2_API trigger_object : public updateable {
public:
	trigger_object();
	virtual ~trigger_object();
};


