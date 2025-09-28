#pragma once

#include <game_object.h>	// sprite_object
#include <input.h>			// playable_object
#include <physics.h>		// physical_object

class arrow : public sprite_object, public playable_object, public physics<2>::physical_object {
public:
	arrow();
	~arrow() = default;
public:
	void init() override;
	void update(const scalar delta_time) override;
public:
	// playable_object overrides
	void process_input(input_engine* const input) override;
};