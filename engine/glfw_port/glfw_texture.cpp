#include "glfw_texture.h"
#include <tsg/io.h>
#include "glfw_converter.h"
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

texture* texture::create_texture() {
	return new glfw_texture();
}

glfw_texture::glfw_texture(int w, int h, const std::string& a) : texture(w, h, a) {  }

void glfw_texture::load(const std::string& asset){
    if (!asset.empty()) {
        if (m_adaptee) {
            unload();
        }

        int width, height;
        int nrChannels;
        unsigned char* data = stbi_load(asset.c_str(), &width, &height, &nrChannels, 0);
        if (data) {

            // Generate texture
            glGenTextures(1, &m_adaptee);
            tsg::print(glGetError());

            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, m_adaptee);
            tsg::print(glGetError());
            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            tsg::print(glGetError());

            // Upload image data to GPU

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            tsg::print(glGetError());
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            tsg::print(glGetError());

            stbi_image_free(data);
        }
        else {
            tsg::print("Error loading texture {}", asset);
        }
    }
}

void glfw_texture::unload() {
    if (m_adaptee) {
        glDeleteTextures(1, &m_adaptee);
        m_adaptee = 0u;
    }
}
