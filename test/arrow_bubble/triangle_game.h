#pragma once

#include <game.h>
#include "arrow.h"
#include "bubble.h"

#define NUMBER_OF_BUBBLE 1

class triangle_game : public game<2, triangle_game>
{
public:
	triangle_game(const std::string&, const unsigned, const unsigned, const unsigned);
	~triangle_game();
public: // override public-methods
	bool initialize();
	void run_game();
	void shutdown();
	void initialize_objects();
	void create_physics();
private: // override private-methods
	void process_input();
	void update_game();
	void generate_output();
private: // object
	arrow m_arrow;
	bubble m_bubbles[NUMBER_OF_BUBBLE];
};

