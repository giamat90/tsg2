#pragma once

#include <game_object.h>
#include <input.h>

class arrow : public dynamic_object, public playable_object {
public:
	arrow();
	~arrow() = default;
public:
	// dynamic_object overrides
	void init() override;
	void update(const float delta_time) override;
public:
	// playable_object overrides
	void process_input(input*) override;
};