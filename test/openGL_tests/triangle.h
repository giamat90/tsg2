#pragma once

#include <game.h>

class triangle : public game
{
public:
	triangle();
	~triangle();
public: // override public-methods
	bool initialize() override;
	void run_game() override;
	void shutdown() override;
	void initialize_objects() override;
private: // override private-methods
	void process_input() override;
	void update_game() override;
	void generate_output() override;
};

