#pragma once

#include "tsg2.h"
#include "window.h"
#include "renderer.h"
#include "input.h"
#include "game_timer.h"
#include "texture.h"
#include "physics.h"
#include "game_event.h"
#include <tsg/types.h>

#ifdef USE_GLFW
template <size_t Dim, typename GameImpl, typename GraphicImpl = glfw_game>
#elifdef USE_VULKAN
template <size_t Dim, typename GameImpl, typename GraphicImpl = vulkan_game>
#endif
class TSG2_API game : public tsg::non_copyable
{
#ifdef USE_GLFW
	using window = glfw_window;
	using renderer = renderer<glfw_renderer>;
	using game_timer = game_timer<glfw_game_timer>;
	using input = input<glfw_window, glfw_input>;
	using game_event = game_event<game, glfw_event>;
#elifdef VULKAN_GAME
	using renderer = renderer<vulkan_renderer>;
	using game_timer = game_timer<vulkan_game_timer>;
	using input = input<vulkan_input>;
	using window = vulkan_window;
#endif
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
		m_window(txt, h, w), m_renderer(m_window), m_timer(fps), m_physics(), m_input(m_window) {
		
	};
	~game() {
		quit(); // quit libraries
	}
	bool initialize() { static_cast<GameImpl*>(this)->initialize(); };
	void run_game() { static_cast<GameImpl*>(this)->run_game(); };
	void shutdown() { static_cast<GameImpl*>(this)->shutdown(); };
public:
	window * const get_window() const { return m_window; }
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
	bool init() { static_cast<GraphicImpl*>(this)->init(); };
	void quit() { static_cast<GraphicImpl*>(this)->quit(); };
protected:
	void set_fps(const unsigned fps) { m_timer.set_fps(fps); }

public:
	inline void add_drawable(drawable* d) { m_renderer.add_drawable(d); };
	inline void add_playable(input::playable_object* o) { m_input.add_playable(o); };
	inline void add_physical_object(physics::physical_object* o) { m_physics.add_physical_object(o); }
protected: // attributes
	GAME_STATE m_state{ GAME_STATE::NONE };
	window m_window;
	input m_input;
	game_timer m_timer;
	renderer m_renderer;
	physics m_physics;
	game_event m_event;

};

