#include "triangle_game.h"
#include <tsg/io.h>
#include <tsg/os.h>
#include "game_event.h"

#include <tsg/logger.h>

#define EXCLUDE_BUBBLE 0
#define EXCLUDE_ARROW NUMBER_OF_BUBBLE > 1 ? 1 : 0

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


triangle_game::triangle_game(const std::string& window_text, const unsigned h, const unsigned w, const unsigned fps) :
	game(window_text, h, w, fps)
{
	tsg::logger::get_instance().write("triangle ctor");
}

triangle_game::~triangle_game() {
	tsg::logger::get_instance().write("triangle dtor");
}

bool triangle_game::initialize() {
	bool res{ false };
	if (init()) {
		create_physics();
		initialize_objects();
		m_state = GAME_STATE::RUNNING;
		res = true;
	}
	else {
		tsg::logger::get_instance().write("Error initializing externals");
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
	m_physics.set_limits({ scalar(m_window.get_width()), scalar(m_window.get_height()) }, scale);
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
	if( GAME_EVENTS::QUIT == get_event()){
		m_state = GAME_STATE::SHUT_DOWN;
	}
	m_input.process_input();
};

void triangle_game::update_game() {
	auto tick = m_timer.tick();
	m_physics.update(tick);
}

void triangle_game::generate_output() {
	m_renderer.render();
	//m_renderer.draw(m_arrow.get_box());
}