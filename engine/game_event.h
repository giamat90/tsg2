#pragma once

#include "tsg2.h"

template <typename GameWindowImpl, typename GameEventImpl>
class TSG2_API game_event{
public:
	enum class GAME_EVENTS : int {
		QUIT
	};
public:
	game_event(const GameWindowImpl& w) : m_window(w) {};
public:
	GAME_EVENTS get_events() { return static_cast<GameEventImpl*>(this)->get_events(); };
protected:
	GameWindowImpl m_window;
};
