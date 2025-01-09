#pragma once

#include <game_object.h>

class paddle : public dynamic_object {
public:
	paddle() : dynamic_object() {}
public: // overrided methods
	void update(const float delta_time) override final;
};