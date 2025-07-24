#pragma once

#include "tsg2.h"
#include "input_engine.h"

class playable_object {
public:
	template <typename InputImpl>
	void process_input(InputImpl* i) {};
};

template<typename PlayableImpl>
class playable : public playable_object {
public:
	template <typename InputImpl>
	void process_input(InputImpl* i) { static_cast<PlayableImpl*>(this)->process_input(i); };

	//bool operator==(const playable_object& other) const = default;
};