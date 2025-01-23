#pragma once

#include <game.h>

class TSG2_API game_event{
public:
	enum class GAME_EVENTS : int {
		QUIT
	};

public:
	static GAME_EVENTS get_events(game* g = nullptr);
};
