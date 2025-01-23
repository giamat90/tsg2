#include "glfw_texture.h"
#include "io/io.h"
#include "glfw_converter.h"

texture* texture::create_texture() {
	return new glfw_texture();
}
