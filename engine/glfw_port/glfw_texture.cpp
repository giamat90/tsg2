#include "glfw_texture.h"
#include <tsg/io.h>
#include "glfw_converter.h"
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

texture* texture::create_texture() {
	return new glfw_texture();
}

glfw_texture::glfw_texture(int w, int h, const std::string& a) : texture(w, h, a) {}

void glfw_texture::load(const std::string& asset){
    if (!asset.empty()) {
        if (m_texture > 0u) {
            unload();
        }

        int width, height;
        int nrChannels;
        unsigned char* data = stbi_load(asset.c_str(), &m_width, &m_height, &nrChannels, 0);

        // Generate texture
        glGenTextures(1, &m_texture);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload image data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset.c_str());

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
}

void glfw_texture::unload() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0u;
    }
}
