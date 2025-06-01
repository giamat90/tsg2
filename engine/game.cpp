#include "game.h"

game::game() {
	m_physics = new physics();
}

game::~game() {
	quit(); // quit libraries
	delete m_renderer;
	delete m_window;
	delete m_input;
	delete m_physics;
	delete m_timer;
}


