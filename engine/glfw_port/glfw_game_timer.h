#pragma once

#include "../tsg2.h"
#include "game_timer.h"

class TSG2_API glfw_game_timer : public game_timer
{
public:
	glfw_game_timer(const unsigned fps);
	const float tick() override;
};

