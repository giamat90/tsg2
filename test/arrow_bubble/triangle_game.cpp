#include "triangle_game.h"
#include <tsg/io.h>
#include "game_event.h"

#define EXCLUDE_BUBBLE 0
#define EXCLUDE_ARROW 1

#if EXCLUDE_BUBBLE
#define INCLUDE_BUBBLE( code ) /* code */
#else
#define INCLUDE_BUBBLE( code ) code
#endif

#if EXCLUDE_ARROW
#define INCLUDE_ARROW( code ) /* code */
#else
#define INCLUDE_ARROW( code ) code
#endif

triangle_game::triangle_game() : game()/*: m_arrow(m_input)*/  {
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
		create_physics();
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

void triangle_game::create_physics() {
	/* TODO */
	geometry::scalar scale = 2.0f;
	m_physics->set_limits(m_window->get_width(), m_window->get_height(), scale);
}

void triangle_game::initialize_objects() {
	/* ToDo */
	// input engine stuff
	INCLUDE_ARROW(add_playable(&m_arrow);)
	// physic engine stuff
	INCLUDE_ARROW(add_physical_object(&m_arrow);)
	// render engine stuff
	/* initialize objects */
	INCLUDE_ARROW(add_drawable(&m_arrow);)
	INCLUDE_ARROW(m_arrow.init();)
	for (std::size_t i = 0u; i < NUMBER_OF_BUBBLE; ++i) {
		INCLUDE_BUBBLE(add_physical_object(&m_bubbles[i]));
		INCLUDE_BUBBLE(add_drawable(&m_bubbles[i]);)
		INCLUDE_BUBBLE(m_bubbles[i].init();)
	}
}

void triangle_game::process_input() {
	if( game_event::GAME_EVENTS::QUIT == game_event::get_events(this)){
		m_state = GAME_STATE::SHUT_DOWN;
	}
	m_input->process_input();
};

void triangle_game::update_game() {
	auto tick = m_timer->tick();
	m_physics->update(tick);
}

void triangle_game::generate_output() {
	m_renderer->render();
	m_renderer->draw(m_arrow.get_box());
}