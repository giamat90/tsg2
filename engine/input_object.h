#pragma once

class input_engine;

class playable_object {
public:
	virtual ~playable_object() = default;
	virtual void process_input(input_engine * const i) = 0;
protected:
};