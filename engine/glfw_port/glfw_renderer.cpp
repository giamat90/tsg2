#include "glfw_renderer.h"
#include "glfw_texture.h"
#include "glfw_font.h"
#include <tsg/io.h>
#include <GLFW/glfw3.h>

glfw_renderer::glfw_renderer(glfw_window * w) : renderer(w) {
	/*
	//// Create SDL renderer
	m_glfw_component = glfw_CreateRenderer(w->m_glfw_window, NULL);

	if (!m_glfw_component)
	{
		throw creation_exception();
	}
	*/
}

glfw_renderer::~glfw_renderer() {
	//glfw_DestroyRenderer(m_glfw_component);
}

// overrided methods
void glfw_renderer::render() {
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Swap front and back buffers */
	if (auto w = dynamic_cast<glfw_window*>(m_window)) {
		glfwSwapBuffers(w->get_adaptee());
	}
}

void glfw_renderer::clear() {
	//glfw_RenderClear(m_glfw_component);
}

void glfw_renderer::set_draw_color(const color& c)
{
	/*
	if (!glfw_SetRenderDrawColor(m_glfw_component, c[color::RGBA::RED], c[color::RGBA::GREEN], c[color::RGBA::BLUE], c[color::RGBA::ALPHA])) {
		throw std::exception();
	}
	*/
}

void glfw_renderer::draw(sprite* s) {

	// Load from file
	//glfw_Surface* surf = IMG_Load(s->get_file().c_str());
}

void glfw_renderer::draw(texture* t) {
	/*
	auto tmp_texture = dynamic_cast<glfw_texture*>(t);
	glfw_FRect dest{
		.x = tmp_texture->get_where().get_x(),
		.y = tmp_texture->get_where().get_y(),
		.w = static_cast<float>(tmp_texture->get_glfw_component()->w),
		.h = static_cast<float>(tmp_texture->get_glfw_component()->h)
	};
	glfw_RenderTexture(m_glfw_component, tmp_texture->get_glfw_component(), NULL, &dest);
	*/
}

void glfw_renderer::draw(font* f) {
	/*
	auto tmp_font = dynamic_cast<glfw_font*>(f);
	if (nullptr == tmp_font) {
		throw(exception("Error converting font in glfw_font"));
	}
	glfw_Surface* tmp_surface = TTF_RenderText_Solid(
		tmp_font->get_glfw_component(), 
		tmp_font->get_text().c_str(), 
		tmp_font->get_text().size(), 
		glfw_converter::convert(tmp_font->get_color()));

	glfw_Texture* tmp_texture = glfw_CreateTextureFromSurface(m_glfw_component, tmp_surface);
	glfw_FRect dest{
		.x = tmp_font->get_where().get_x(),
		.y = tmp_font->get_where().get_y(),
		.w = static_cast<float>(tmp_surface->w),
		.h = static_cast<float>(tmp_surface->h)
	};
	glfw_RenderTexture(m_glfw_component, tmp_texture, NULL, &dest);

	glfw_DestroyTexture(tmp_texture);
	glfw_DestroySurface(tmp_surface);
	*/
}

void glfw_renderer::draw(geometry::rectangle& r) {
	/*
	glfw_FRect tmp{ r.get_x(), r.get_y(), r.get_w(), r.get_h() };
	if (!glfw_RenderFillRect(m_glfw_component, &tmp)) {
		throw std::exception();
	}
	*/
}