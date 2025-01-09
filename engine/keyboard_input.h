#pragma once

#include "tsg2.h"
#include "input.h"

enum class INPUT_KEY : int {
		KEY_ESC,
		KEY_RET,
		KEY_LCTRL,
		KEY_RCTRL,
		KEY_LSHIFT,
		KEY_RSHIFT,
		KEY_RALT,
		KEY_LALT,
		KEY_TAB,
		KEY_Q,
		KEY_W,
		KEY_E,
		KEY_R,
		KEY_T,
		KEY_Y,
		KEY_U,
		KEY_I,
		KEY_O,
		KEY_P,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_Z,
		KEY_X,
		KEY_C,
		KEY_V,
		KEY_B,
		KEY_N,
		KEY_M,
		MAX_KEY = KEY_M
	};

class TSG2_API keyboard_input : public input {
public:
	keyboard_input() : input() {};
	virtual ~keyboard_input() {};
	virtual bool is_key_pressed(const INPUT_KEY key) = 0;
};