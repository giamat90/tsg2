#pragma once

#include "tsg2.h"
#include "texture.h"

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
public:
	texture* get_texture() { return m_texture; }
	void set_texture(texture* r) { m_texture = r; }
protected:
	texture* m_texture{ nullptr };
};

class TSG2_API updateable : public game_object {
public:
	updateable();
	virtual ~updateable();
	virtual void update(const float delta_time) = 0;
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


