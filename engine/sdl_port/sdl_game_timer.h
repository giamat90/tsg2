#pragma once

#include "../tsg2.h"
#include "game_timer.h"

class TSG2_API sdl_game_timer : public game_timer
{
public:
	sdl_game_timer(const unsigned fps) : game_timer(fps) {}
	const float tick() override;
};

