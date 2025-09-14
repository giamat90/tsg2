#include "game_object.h"

game_object::game_object() = default;
game_object::~game_object() = default;

drawable::drawable() = default;
drawable::~drawable() = default;

updateable::updateable() = default;
updateable::~updateable() = default;

static_object::static_object() = default;
static_object::~static_object() = default;

dynamic_object::dynamic_object() = default;
dynamic_object::~dynamic_object() = default;

trigger_object::trigger_object() = default;
trigger_object::~trigger_object() = default;

sprite_object::~sprite_object() {
	if (m_sprite) {
		delete m_sprite;
		m_sprite = nullptr;
	}
}

void sprite_object::update(const position& p, const angle a) {
	if (m_sprite) {
		m_sprite->set_where(p);
		m_sprite->set_rotation(a);
	}
}

