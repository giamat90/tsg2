#pragma once

#include "tsg2.h"
#include "texture.h"
#include "renderer.h"

class TSG2_API game_object {
public:
	game_object();
	virtual ~game_object();
};

class TSG2_API drawable : public game_object {
public:
	drawable();
	virtual ~drawable();
	void draw() {
		if (m_renderer && m_texture) {
			m_renderer->draw(m_texture);
		}
	};
public:
	void set_renderer(renderer* r) { m_renderer = r; }
	void set_texture(texture* r) { m_texture = r; }
protected:
	texture* m_texture{ nullptr };
	renderer* m_renderer{ nullptr };
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
	trigger_object();
	virtual ~trigger_object();
};

/* physical_object to detect collisions */
class TSG2_API physical_object : public game_object {
public:
	physical_object();
	virtual ~physical_object();
protected:

};