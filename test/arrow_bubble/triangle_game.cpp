#include "triangle_game.h"
#include <tsg/io.h>
#include "game_event.h"

triangle_game::triangle_game() /*: m_arrow(m_input)*/  {
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
		create_input();
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
	while (GAME_STATE::RUNNING == m_state) {
		process_input();
		update_game();
		generate_output();
	}
}

void triangle_game::shutdown() {
	/* ToDo */
	quit();
}

void triangle_game::initialize_objects() {
	/* ToDo */
	m_arrow.init();
	m_bubble.init();
	// render engine stuff
	add_drawable(&m_arrow);
	add_drawable(&m_bubble);
	// input engine stuff
	add_playable(&m_arrow);
	// physic engine stuff
	add_physical_object(&m_arrow);

}

void triangle_game::process_input() {
	if( game_event::GAME_EVENTS::QUIT == game_event::get_events(this)){
		m_state = GAME_STATE::SHUT_DOWN;
	}
	m_input->process_input();
	/* ToDo */
	// process keyboard - inputs
	// process mose - inputs
	// process joystick - inputs
};

void triangle_game::update_game() {
	auto tick = m_timer->tick();
	m_physics->update(tick);
	m_bubble.update(tick);
	m_arrow.update(tick);
	/* ToDo */
}

void triangle_game::generate_output() {
	m_renderer->render();
	/* ToDo */
}