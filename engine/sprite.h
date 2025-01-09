#pragma once

#include "tsg2.h"
#include "texture.h"
#include <string>

class TSG2_API sprite : public texture {
public:
	sprite() = default;
	virtual ~sprite() = default;
	sprite(const std::string& path) : m_file_path(path) {};
	inline std::string get_file() { return m_file_path; }
protected:
	std::string m_file_path{};

};
