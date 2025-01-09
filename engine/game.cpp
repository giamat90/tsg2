#include "game.h"

game::game() {}

game::~game() {
	quit(); // quit libraries
	delete m_renderer;
	delete m_window;
	delete m_input;
	delete m_timer;
}


