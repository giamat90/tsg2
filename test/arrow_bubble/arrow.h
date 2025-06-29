#pragma once

#include <game_object.h>
#include <input.h>
#include <physics.h>

class arrow : public dynamic_object, public input::playable_object, public physics::physical_object {
public:
	arrow();
	~arrow() = default;
public:
	// dynamic_object overrides
	void init() override;
	void update(const scalar delta_time) override;
public:
	// playable_object overrides
	void process_input(input*) override;
};