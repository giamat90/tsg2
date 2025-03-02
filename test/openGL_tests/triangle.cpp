#include "triangle.h"
#include <tsg/io.h>
#include "game_event.h"

triangle::triangle() {
	tsg::print("triangle ctor");
}

triangle::~triangle() {
	tsg::print("triangle dtor");
}

bool triangle::initialize() {
	bool res{ false };
	if (initialize_externals()) {
		create_window("triangle.exe", 1024, 768);
		create_renderer();
		create_keyboard_input();
		create_timer(60u);
		initialize_objects();
		m_state = GAME_STATE::RUNNING;
		res = true;
	}
	else {
		tsg::print("Error initializing externals");
	}
	return res;
}

void triangle::run_game() {
	while (GAME_STATE::SHUT_DOWN != m_state) {
		process_input();
		update_game();
		generate_output();
	}
}

void triangle::shutdown() {
	/* ToDo */
}

void triangle::initialize_objects() {
	/* ToDo */
}

void triangle::process_input() {
	if( game_event::GAME_EVENTS::QUIT == game_event::get_events(this)){
		m_state = GAME_STATE::STOPPING;
	}
	/* ToDo */
	// process keyboard - inputs
	// process mose - inputs
	// process joystick - inputs
};

void triangle::update_game() {
	/* ToDo */
	auto tick = m_timer->tick();
	tsg::print("tick={}", tick);
}

void triangle::generate_output() {
	m_renderer->render();
	/* ToDo */
}