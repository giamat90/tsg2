#include "pong.h"
#include "keyboard_input.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <thread>
#include <chrono>
#include <tsg/io.h>

using geometry::rectangle;
using geometry::AXES;

constexpr int thickness = 15;
constexpr float paddleH = 100.0f;

pong::pong() : game() {
}

bool pong::initialize() {
	bool res{ false };
	if (initialize_externals()) {
		create_window("pong.exe", 1024, 768);
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
};

void pong::initialize_objects() {
	m_field.set_renderer(m_renderer);
	m_ball.set_renderer(m_renderer);
	// creating game-objects
	m_field.set_texture(create_texture("C:\\Workspace\\GiaMat90\\pong\\Project1\\Assets\\paddle-court_dark_green.png"));
	m_ball.set_texture(create_texture("C:\\Workspace\\GiaMat90\\pong\\Project1\\Assets\\paddle-tennis-ball-yellow.png"));
	// score counter
	m_score = font::create_font();
	m_score->load_font("C:\\Workspace\\GiaMat90\\pong\\Project1\\Font\\Carlito-Regular.ttf", 30u);
	m_score->set_where(font::text_position(800, 50));
	m_score->set_color(color(255, 255, 255, 255));
	m_score->set_text("BOUNCES: 0");
}

void pong::run_game() {
	while (GAME_STATE::RUNNING == m_state) {
		process_input();
		update_game();
		generate_output();
	}
};

void pong::shutdown() {};

void pong::process_input() {
	// GM - ToDo: generalize
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_EVENT_QUIT:
			m_state = GAME_STATE::STOPPING;
			break;
		}
	}

	// Get state of keyboard
	auto keyboard = dynamic_cast<keyboard_input*>(m_input);
	if (keyboard) {
		if (keyboard->is_key_pressed(INPUT_KEY::KEY_ESC))
		{
			m_state = GAME_STATE::STOPPING;
		}

		// Update paddle direction based on W/S keys
		m_paddle_dir = 0;
		if (keyboard->is_key_pressed(INPUT_KEY::KEY_W))
		{
			m_paddle_dir -= 1;
		}
		if (keyboard->is_key_pressed(INPUT_KEY::KEY_S))
		{
			m_paddle_dir += 1;
		}
	}
};

void pong::update_game() 
{
	const float delta_time = m_timer->tick();
	// Update paddle position based on direction
	if (m_paddle_dir != 0)
	{
		m_paddle_pos[AXES::Y] += m_paddle_dir * 300.0f * delta_time;
		// Make sure paddle doesn't move off screen!
		if (m_paddle_pos[AXES::Y] < (paddleH / 2.0f + thickness))
		{
			m_paddle_pos[AXES::Y] = paddleH / 2.0f + thickness;
		}
		else if (m_paddle_pos[AXES::Y] > (768.0f - paddleH / 2.0f - thickness))
		{
			m_paddle_pos[AXES::Y] = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	m_ball_pos[AXES::X] += m_ball_vel.get_x() * delta_time;
	m_ball_pos[AXES::Y] += m_ball_vel.get_y() * delta_time;

	// Bounce if needed
	// 
	auto increase_ball_vel = [&]() {
		if (m_ball_vel[AXES::X] > 0) {
			++m_ball_vel[AXES::X];
		}
		else {
			--m_ball_vel[AXES::X];
		}
		if (m_ball_vel[AXES::Y] > 0) {
			++m_ball_vel[AXES::Y];
		}
		else {
			--m_ball_vel[AXES::Y];
		}
	};
	// Did we intersect with the paddle?
	float diff = m_paddle_pos.get_y() - m_ball_pos.get_y();
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		m_ball_pos.get_x() <= 25.0f && m_ball_pos.get_x() >= 20.0f &&
		// The ball is moving to the left
		m_ball_vel.get_x() < 0.0f)
	{
		m_ball_vel[AXES::X] *= -1.0f;
		++m_bounces;
		increase_ball_vel();
	}
	// Did the ball go off the screen? (if so, end game)
	else if (m_ball_pos.get_x() <= 0.0f)
	{
		m_state = GAME_STATE::STOPPING;
	}
	// Did the ball collide with the right wall?
	else if (m_ball_pos.get_x() >= (1024.0f - thickness) && m_ball_vel.get_x() > 0.0f)
	{
		m_ball_vel[AXES::X] *= -1.0f;
		increase_ball_vel();
	}

	// Did the ball collide with the top wall?
	if (m_ball_pos.get_y() <= thickness && m_ball_vel.get_y() < 0.0f)
	{
		m_ball_vel[AXES::Y] *= -1;
		increase_ball_vel();
	}
	// Did the ball collide with the bottom wall?
	else if (m_ball_pos.get_y() >= (768 - thickness) && m_ball_vel.get_y() > 0.0f)
	{
		m_ball_vel[AXES::Y] *= -1;
		increase_ball_vel();
	}
};

void pong::generate_output() {
	// Set draw color to blue
	m_renderer->set_draw_color(color(0u, 0u, 255u, 255u));

	// Clear back buffer
	m_renderer->clear();

	m_field.draw();
	m_ball.draw();

	// Draw walls
	m_renderer->set_draw_color(color(255u, 255u, 255u, 255u));

	// Draw top wall
	rectangle wall(0, 0, 1024, thickness);
	m_renderer->draw(wall);

	// Draw bottom wall
	wall.set_y(768 - thickness);
	m_renderer->draw(wall);

	// Draw right wall
	wall.set_x(1024 - thickness);
	wall.set_y(0);
	wall.set_w(thickness);
	wall.set_h(1024);
	m_renderer->draw(wall);

	// Draw paddle
	rectangle paddle{
		m_paddle_pos.get_x(),
		m_paddle_pos.get_y() - static_cast<unsigned>(paddleH) / 2,
		thickness,
		static_cast<unsigned>(paddleH)
	};
	m_renderer->draw(paddle);

	// Draw ball
	rectangle ball{
		m_ball_pos.get_x() - thickness / 2,
		m_ball_pos.get_y() - thickness / 2,
		thickness,
		thickness
	};
	m_renderer->draw(ball);

	m_score->set_text("BOUNCES: " + std::to_string(m_bounces));
	m_renderer->draw(m_score);
	

	// Swap front buffer and back buffer
	m_renderer->render();
};


