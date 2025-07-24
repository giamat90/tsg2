#pragma once

#include "tsg2.h"
//#include "window.h"
//#include "renderer.h"
#include "input.h"
#include "input_object.h"
#include "game_timer.h"
#include "texture.h"
#include "physics.h"
#include "game_event.h"
#include <tsg/types.h>

#ifdef USE_GLFW
#include "glfw_port/glfw_game.h"
#include "glfw_port/glfw_renderer.h"
#include "glfw_port/glfw_game_timer.h"
#include "glfw_port/glfw_input.h"
#include "glfw_port/glfw_window.h"
#include "glfw_port/glfw_event.h"

using window_impl = glfw_window;
using renderer_impl = glfw_renderer;
using input_impl = glfw_input;
using game_event_impl = glfw_event;
using game_timer_impl = glfw_game_timer;

template <size_t Dim, typename GameImpl, typename GraphicImpl = glfw_game>
#elifdef VULKAN_GAME
/* TODO */
#elifdef GLES_GAME
#endif
class game : public tsg::non_copyable, public GraphicImpl
{
	using physics = physics<Dim>;
public:
	enum class GAME_STATE {
		NONE,
		INITIALIZING,
		RUNNING,
		STOPPING,
		SHUT_DOWN
	};
	game(const std::string& txt, const unsigned h, const unsigned w, const unsigned fps) : 
		m_window(txt, h, w), m_renderer(&m_window), m_input(&m_window), m_event(&m_window), m_timer(fps), m_physics() {
		
	};
	~game() {
		quit(); // quit libraries
	}
	bool initialize() { static_cast<GameImpl*>(this)->initialize(); };
	void run_game() { static_cast<GameImpl*>(this)->run_game(); };
	void shutdown() { static_cast<GameImpl*>(this)->shutdown(); };
public:
	window * const get_window() const { return &m_window; }
	GAME_EVENTS get_event() { return m_event.get_events(); };
protected: // virtual methods
	void process_input() { static_cast<GameImpl*>(this)->process_input(); };
	/* Calculate elapsed time and update game-object states */
	void update_game() { static_cast<GameImpl*>(this)->update_game(); };
	/* Create video output */
	void generate_output() { static_cast<GameImpl*>(this)->generate_output(); };
	/* Set game physics proprieties */
	void create_physics() { static_cast<GameImpl*>(this)->create_physics(); };
	/* initialize objects */
	void initialize_objects() { static_cast<GameImpl*>(this)->initialize_objects(); };
protected:
	/* Methods for initialize graphic engine */
	bool init() { return static_cast<GraphicImpl*>(this)->init(); };
	void quit() { static_cast<GraphicImpl*>(this)->quit(); };
protected:
	void set_fps(const unsigned fps) { m_timer.set_fps(fps); }

public:
	inline void add_drawable(drawable* d) { m_renderer.add_drawable(d); };
	inline void add_playable(playable_object* o) { m_input.add_playable(o); };
	inline void add_physical_object(physics::physical_object* o) { m_physics.add_physical_object(o); }
protected: // attributes
	GAME_STATE m_state{ GAME_STATE::NONE };
	window_impl m_window;
	renderer_impl m_renderer;
	input_impl m_input;
	game_event_impl m_event;
	game_timer_impl m_timer;
	physics m_physics;

};

