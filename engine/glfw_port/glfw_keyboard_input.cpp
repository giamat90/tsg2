#include "glfw_keyboard_input.h"


glfw_keyboard_input::glfw_keyboard_input() : keyboard_input() {
	//glfwSetKeyCallback(window, key_callback);

}

bool glfw_keyboard_input::is_key_pressed(const INPUT_KEY key)
{
	// Get state of keyboard
	//return glfw_GetKeyboardState(NULL)[m_keyboard_glfw_mapping.at(key)];
	return false;
}