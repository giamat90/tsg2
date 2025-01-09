#include "sdl_keyboard_input.h"

bool sdl_keyboard_input::is_key_pressed(const INPUT_KEY key)
{
	// Get state of keyboard
	return SDL_GetKeyboardState(NULL)[m_keyboard_sdl_mapping.at(key)];
}