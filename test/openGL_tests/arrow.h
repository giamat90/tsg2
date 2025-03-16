#pragma once

#include <game_object.h>

class arrow : public dynamic_object {
public:
	arrow();
	~arrow() = default;
public:
	void update(const float delta_time) override;
};