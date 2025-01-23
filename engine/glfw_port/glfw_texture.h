#pragma once

#include "../tsg2.h"
#include "glfw_adaptor.h"
#include "texture.h"
#include "asset.h"

class TSG2_API glfw_texture : public texture
{
public:
	glfw_texture() = default ;
	virtual ~glfw_texture() {};
};

