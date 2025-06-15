#pragma once

#include "tsg2.h"
#include <exception>
#include <string>
#include <iostream>

class TSG2_API window {
public:
	using width_t = std::uint32_t;
	using height_t = std::uint32_t;
public:
	class window_exception : public std::exception {
		const char* what() { return "window exception"; }
	};
	class create_exception : public window_exception {
		const char* what() { return "create exception"; }
	};
	struct position {
		width_t x{};
		height_t y{};
	};
	window(std::string title, width_t w, height_t h) :
		m_title(title), m_width(w), m_height(h) {}
	virtual ~window() = default;	
	height_t get_height() const { return m_height; }
	width_t get_width() const { return m_width; }
protected:
	std::string m_title;
	width_t m_width;
	height_t m_height;
	position m_start_pos;
};
