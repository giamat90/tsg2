#pragma once

#include <game_object.h>

class bubble : public dynamic_object {
public:
	bubble();
	~bubble() = default;
public:
	void init() override;
	void update(const float delta_time) override;
};