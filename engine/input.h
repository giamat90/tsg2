#pragma once

#include "tsg2.h"
#include "window.h"
#include <vector>



class TSG2_API input {
public:
	class TSG2_API playable_object {
	public:
		playable_object() = default;
		virtual ~playable_object() = default;
		virtual void process_input(input*) = 0;
	};
public:
	enum class INPUT_TYPE {
		KEYBOARD,
		MOUSE,
		GAMEPAD,
		JOYSTICK
	};
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
		KEY_SPACE,
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
	enum class INPUT_MOUSE {
		LEFT,
		RIGHT
	};
public:
	input(window* w) : m_window(w) {};
	virtual ~input() = default;
public:
	void process_input() {
		for (auto p : m_playables) {			
			p->process_input(this);
		}
	}
public:
	// kind of inputs events
	// keyboard
	virtual bool is_key_pressed(const INPUT_KEY key) = 0;
	// mouse
	virtual bool is_mouse_clicked(const INPUT_MOUSE side) = 0;
	virtual bool is_mouse_pressed(const INPUT_MOUSE side) = 0;
	virtual bool is_mouse_released(const INPUT_MOUSE side) = 0;
	// gamepad
	/* TODO */
	// joystick
	/* TODO */
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_playable(playable_object* d) {
		m_playables.push_back(d);
	}
protected:
	window* m_window{ nullptr };
	std::vector<playable_object*> m_playables;
};
