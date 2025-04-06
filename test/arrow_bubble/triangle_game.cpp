#include "triangle_game.h"
#include <tsg/io.h>
#include "game_event.h"

triangle_game::triangle_game() {
	tsg::print("triangle ctor");
}

triangle_game::~triangle_game() {
	tsg::print("triangle dtor");
}

bool triangle_game::initialize() {
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

void triangle_game::run_game() {
	while (GAME_STATE::SHUT_DOWN != m_state) {
		process_input();
		update_game();
		generate_output();
	}
}

void triangle_game::shutdown() {
	/* ToDo */
}

void triangle_game::initialize_objects() {
	/* ToDo */
	m_arrow.init();
	m_bubble.init();
	//main_character.set_renderer(m_renderer);
	m_renderer->add_drawable(&m_arrow);
	m_renderer->add_drawable(&m_bubble);
}

void triangle_game::process_input() {
	if( game_event::GAME_EVENTS::QUIT == game_event::get_events(this)){
		m_state = GAME_STATE::STOPPING;
	}
	/* ToDo */
	// process keyboard - inputs
	// process mose - inputs
	// process joystick - inputs
};

void triangle_game::update_game() {
	/* ToDo */
	auto tick = m_timer->tick();
	m_bubble.update(tick);
	m_arrow.update(tick);
}

void triangle_game::generate_output() {
	m_renderer->render();
	//main_character.draw();
	/* ToDo */
}