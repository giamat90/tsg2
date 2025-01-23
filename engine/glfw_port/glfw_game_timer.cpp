#include "glfw_game_timer.h"

const float glfw_game_timer::tick() {
	/*
	static int m_ticks_elapsed = static_cast<int>(glfw_GetTicks()); // ms since the library started
	const int tick_msec{ 1000 / static_cast<int>(m_fps) }; // minimum time unit in ms
	while ((m_ticks_elapsed + tick_msec - static_cast<int>(glfw_GetTicks())) > 0) {}
	// time elapsed
	float delta_time = (glfw_GetTicks() - m_ticks_elapsed) / 1000.0f;
	// store time for next tick
	m_ticks_elapsed = static_cast<unsigned>(glfw_GetTicks()); 
	// upper limit the time
	const float threshold_sec{ 1.0f / m_fps }; 
	return delta_time > threshold_sec ? threshold_sec : delta_time;
	*/
	return 0.0f;
}