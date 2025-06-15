#pragma once

#include <game.h>
#include "arrow.h"
#include "bubble.h"

class triangle_game : public game
{
public:
	triangle_game();
	~triangle_game();
public: // override public-methods
	bool initialize() override;
	void run_game() override;
	void shutdown() override;
	void initialize_objects() override;
	void create_physics() override;
private: // override private-methods
	void process_input() override;
	void update_game() override;
	void generate_output() override;
private: // object
	arrow m_arrow;
	bubble m_bubble;
};

