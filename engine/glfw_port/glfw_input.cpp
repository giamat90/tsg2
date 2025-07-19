#include "../input.h"
#include "glfw_input.h"
#include <map>
#include <GLFW/glfw3.h>
#include "gl_utility.h" // gl_check_error
#include <cassert>


static std::map<input<glfw_input>::INPUT_KEY, int> m_keyboard_glfw_mapping{
	{ input<glfw_input>::INPUT_KEY::KEY_ESC, GLFW_KEY_ESCAPE},
	{ input<glfw_input>::INPUT_KEY::KEY_RET, GLFW_KEY_ENTER },
	{ input<glfw_input>::INPUT_KEY::KEY_LCTRL, GLFW_KEY_LEFT_CONTROL },
	{ input<glfw_input>::INPUT_KEY::KEY_RCTRL, GLFW_KEY_RIGHT_CONTROL },
	{ input<glfw_input>::INPUT_KEY::KEY_LSHIFT, GLFW_KEY_LEFT_SHIFT },
	{ input<glfw_input>::INPUT_KEY::KEY_RSHIFT, GLFW_KEY_RIGHT_SHIFT },
	{ input<glfw_input>::INPUT_KEY::KEY_LALT, GLFW_KEY_LEFT_ALT },
	{ input<glfw_input>::INPUT_KEY::KEY_RALT, GLFW_KEY_RIGHT_ALT },
	{ input<glfw_input>::INPUT_KEY::KEY_TAB, GLFW_KEY_TAB },
	{ input<glfw_input>::INPUT_KEY::KEY_SPACE, GLFW_KEY_SPACE },
	{ input<glfw_input>::INPUT_KEY::KEY_LEFT, GLFW_KEY_LEFT },
	{ input<glfw_input>::INPUT_KEY::KEY_RIGHT, GLFW_KEY_RIGHT },
	{ input<glfw_input>::INPUT_KEY::KEY_DOWN, GLFW_KEY_DOWN },
	{ input<glfw_input>::INPUT_KEY::KEY_UP, GLFW_KEY_UP },
	{ input<glfw_input>::INPUT_KEY::KEY_Q, GLFW_KEY_Q },
	{ input<glfw_input>::INPUT_KEY::KEY_W, GLFW_KEY_W },
	{ input<glfw_input>::INPUT_KEY::KEY_E, GLFW_KEY_E },
	{ input<glfw_input>::INPUT_KEY::KEY_R, GLFW_KEY_R },
	{ input<glfw_input>::INPUT_KEY::KEY_T, GLFW_KEY_T },
	{ input<glfw_input>::INPUT_KEY::KEY_Y, GLFW_KEY_Y },
	{ input<glfw_input>::INPUT_KEY::KEY_U, GLFW_KEY_U },
	{ input<glfw_input>::INPUT_KEY::KEY_I, GLFW_KEY_I },
	{ input<glfw_input>::INPUT_KEY::KEY_O, GLFW_KEY_O },
	{ input<glfw_input>::INPUT_KEY::KEY_P, GLFW_KEY_P },
	{ input<glfw_input>::INPUT_KEY::KEY_A, GLFW_KEY_A },
	{ input<glfw_input>::INPUT_KEY::KEY_S, GLFW_KEY_S },
	{ input<glfw_input>::INPUT_KEY::KEY_D, GLFW_KEY_D },
	{ input<glfw_input>::INPUT_KEY::KEY_F, GLFW_KEY_F },
	{ input<glfw_input>::INPUT_KEY::KEY_G, GLFW_KEY_G },
	{ input<glfw_input>::INPUT_KEY::KEY_H, GLFW_KEY_H },
	{ input<glfw_input>::INPUT_KEY::KEY_J, GLFW_KEY_J },
	{ input<glfw_input>::INPUT_KEY::KEY_K, GLFW_KEY_K },
	{ input<glfw_input>::INPUT_KEY::KEY_L, GLFW_KEY_L },
	{ input<glfw_input>::INPUT_KEY::KEY_Z, GLFW_KEY_Z },
	{ input<glfw_input>::INPUT_KEY::KEY_X, GLFW_KEY_X },
	{ input<glfw_input>::INPUT_KEY::KEY_C, GLFW_KEY_C },
	{ input<glfw_input>::INPUT_KEY::KEY_V, GLFW_KEY_V },
	{ input<glfw_input>::INPUT_KEY::KEY_B, GLFW_KEY_B },
	{ input<glfw_input>::INPUT_KEY::KEY_N, GLFW_KEY_N },
	{ input<glfw_input>::INPUT_KEY::KEY_M, GLFW_KEY_M },
};

glfw_input::glfw_input(glfw_window* w) : input(w) {}
glfw_input::~glfw_input(){}

// overloaded methods
bool glfw_input::is_key_pressed(const INPUT_KEY key) {
	bool res{ false };
	if (auto w = dynamic_cast<glfw_window*>(m_window))
	{
		res = glfwGetKey(w->get_adaptee(), m_keyboard_glfw_mapping[key]) == GLFW_PRESS;
		gl_check_error();
	}
	return res;
};
// mouse
bool glfw_input::is_mouse_clicked(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}
bool glfw_input::is_mouse_pressed(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}
bool glfw_input::is_mouse_released(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}