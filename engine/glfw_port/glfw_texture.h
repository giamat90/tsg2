#pragma once

#include "../tsg2.h"
#include "glfw_adaptor.h"
#include "texture.h"
#include "asset.h"
#include <glad/gl.h>

class TSG2_API glfw_texture : public texture
{
public:
	glfw_texture(int width = 0, int height = 0, const std::string& asset = "");
	virtual ~glfw_texture() = default;
	void load(const std::string& asset = "") override;
	void unload() override;
private:
	GLuint m_texture{};
};

