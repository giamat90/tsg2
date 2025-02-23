#include "glfw_texture.h"
#include <tsg/io.h>
#include "glfw_converter.h"

texture* texture::create_texture() {
	return new glfw_texture();
}
