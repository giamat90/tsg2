#pragma once

#include <game_object.h>
#include <physics.h>

class bubble : public dynamic_object, public physics::physical_object {
public:
	bubble();
	~bubble() = default;
public:
	void init() override;
	void update(const float delta_time) override;
};