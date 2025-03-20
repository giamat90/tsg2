#pragma once

#include "../tsg2.h"
#include <tsg/types.h>
#include "glfw_adaptor.h"
#include "texture.h"
#include "asset.h"
#include <glad/gl.h>

class TSG2_API glfw_texture : public texture, public tsg::adapter<GLuint>
{
public:
	glfw_texture(int width = 0, int height = 0, const std::string& asset = "");
	virtual ~glfw_texture() = default;
	void load(const std::string& asset = "") override;
	void unload() override;
	GLuint get_adaptee() { return m_id; }
protected:
	GLuint m_id;
};

