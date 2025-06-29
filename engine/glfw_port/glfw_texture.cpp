#include "glfw_texture.h"
#include "glfw_converter.h"
#include "../logger.h"
#include <tsg/io.h>         // print
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // stbi_load
#include "gl_utility.h" // gl_check_error

texture* texture::create_texture() {
	return new glfw_texture();
}

glfw_texture::glfw_texture(int w, int h, const std::string& a) : texture(w, h, a) {  }
glfw_texture::~glfw_texture() { unload(); }

void glfw_texture::load(const std::string& asset){
    if (!asset.empty()) {
        if (m_adaptee) {
            unload();
        }

        int nrChannels;
        unsigned char* data = stbi_load(asset.c_str(), &m_width, &m_height, &nrChannels, 0);
        if (data) {
            GLenum rgba_format;
            switch (nrChannels)
            {
            case 1:
                rgba_format = GL_RED;
                break;
            case 3:
                rgba_format = GL_RGB;
                break;
            case 4:
                rgba_format = GL_RGBA;
                break;
            default:
                rgba_format = GL_RGB;
                break;
            }
            // Generate texture
            glGenTextures(1, &m_adaptee);
            gl_check_error(__FILE__, __LINE__);

            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, m_adaptee);
            gl_check_error(__FILE__, __LINE__);

            // Upload image data to GPU
            glTexImage2D(GL_TEXTURE_2D, 0, rgba_format, m_width, m_height, 0, rgba_format, GL_UNSIGNED_BYTE, data);
            gl_check_error(__FILE__, __LINE__);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            gl_check_error(__FILE__, __LINE__);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            gl_check_error(__FILE__, __LINE__);

        }
        else {
            logger::get_istance().write("Error loading texture {}, reason {}", asset, stbi_failure_reason());
        }
        stbi_image_free(data);
    }
}

void glfw_texture::unload() {
    if (m_adaptee) {
        glDeleteTextures(1, &m_adaptee);
        m_adaptee = 0u;
    }
}
