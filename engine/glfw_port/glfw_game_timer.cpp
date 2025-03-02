#include "glfw_game_timer.h"
#include <tsg/io.h>

glfw_game_timer::glfw_game_timer(const unsigned fps) : game_timer(fps) {
	glfwSetTime(0.0f);
}

const float glfw_game_timer::tick() {
	static float m_ticks_elapsed = glfwGetTime(); // convert in ms
	static const float tick_sec{ 1 / static_cast<float>(m_fps) }; // minimum time unit in ms
		while ( glfwGetTime() - m_ticks_elapsed < tick_sec){}
	// time elapsed
	float delta_time = (glfwGetTime() - m_ticks_elapsed);
	// store time for next tick
	m_ticks_elapsed = glfwGetTime();
	// upper limit the time
	const float threshold_sec{ 1.0f / m_fps };
	return delta_time > threshold_sec ? threshold_sec : delta_time;
}