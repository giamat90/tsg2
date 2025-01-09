#pragma once

#include <game.h>
#include <geometry.h>
#include <font.h>
#include "field.h"
#include "ball.h"

class pong: public game
{
public:
	using pong_vector = geometry::vector<2u>;

public:
	pong();
	bool initialize() override;
	void run_game() override;
	void shutdown() override;
	void initialize_objects() override;
private:
	void process_input() override;
	void update_game() override;
	void generate_output() override;
private:
	pong_vector m_paddle_pos{10.0f, 768.0f / 2.0f };
	pong_vector m_ball_pos{1024.0f / 2.0f, 768.0f / 2.0f };
	pong_vector m_ball_vel{-300.0f, 350.0f};
	int m_paddle_dir{};
	unsigned m_bounces{}; // score for single player
	font* m_score{ nullptr };
private:
	field m_field;
	ball m_ball;

};

