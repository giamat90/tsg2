#pragma once

#include "../tsg2.h"
#include "keyboard_input.h"
#include <map>

class TSG2_API glfw_keyboard_input : public keyboard_input
{
public:
	bool is_key_pressed(const INPUT_KEY key) override;
private:
#if 0
	std::map<INPUT_KEY, glfw_Scancode> m_keyboard_glfw_mapping{
		{INPUT_KEY::KEY_ESC, glfw_SCANCODE_ESCAPE},
		{INPUT_KEY::KEY_RET, glfw_SCANCODE_RETURN},
		{INPUT_KEY::KEY_LCTRL, glfw_SCANCODE_LCTRL},
		{INPUT_KEY::KEY_RCTRL, glfw_SCANCODE_RCTRL},
		{INPUT_KEY::KEY_LSHIFT, glfw_SCANCODE_LSHIFT},
		{INPUT_KEY::KEY_RSHIFT, glfw_SCANCODE_RSHIFT},
		{INPUT_KEY::KEY_LALT, glfw_SCANCODE_LALT},
		{INPUT_KEY::KEY_RALT, glfw_SCANCODE_RALT},
		{INPUT_KEY::KEY_TAB, glfw_SCANCODE_TAB},
		{INPUT_KEY::KEY_Q, glfw_SCANCODE_Q},
		{INPUT_KEY::KEY_W, glfw_SCANCODE_W},
		{INPUT_KEY::KEY_E, glfw_SCANCODE_E},
		{INPUT_KEY::KEY_R, glfw_SCANCODE_R},
		{INPUT_KEY::KEY_T, glfw_SCANCODE_T},
		{INPUT_KEY::KEY_Y, glfw_SCANCODE_Y},
		{INPUT_KEY::KEY_U, glfw_SCANCODE_U},
		{INPUT_KEY::KEY_I, glfw_SCANCODE_I},
		{INPUT_KEY::KEY_O, glfw_SCANCODE_O},
		{INPUT_KEY::KEY_P, glfw_SCANCODE_P},
		{INPUT_KEY::KEY_A, glfw_SCANCODE_A},
		{INPUT_KEY::KEY_S, glfw_SCANCODE_S},
		{INPUT_KEY::KEY_D, glfw_SCANCODE_D},
		{INPUT_KEY::KEY_F, glfw_SCANCODE_F},
		{INPUT_KEY::KEY_G, glfw_SCANCODE_G},
		{INPUT_KEY::KEY_H, glfw_SCANCODE_H},
		{INPUT_KEY::KEY_J, glfw_SCANCODE_J},
		{INPUT_KEY::KEY_K, glfw_SCANCODE_K},
		{INPUT_KEY::KEY_L, glfw_SCANCODE_L},
		{INPUT_KEY::KEY_Z, glfw_SCANCODE_Z},
		{INPUT_KEY::KEY_X, glfw_SCANCODE_X},
		{INPUT_KEY::KEY_C, glfw_SCANCODE_C},
		{INPUT_KEY::KEY_V, glfw_SCANCODE_V},
		{INPUT_KEY::KEY_B, glfw_SCANCODE_B},
		{INPUT_KEY::KEY_N, glfw_SCANCODE_N},
		{INPUT_KEY::KEY_M, glfw_SCANCODE_M},
	};
#endif
};

