#include "glfw_renderer.h"
#include "glfw_texture.h"
#include "glfw_font.h"
#include <geometry.h>
#include <tsg/io.h>	// print
#include <tsg/os.h> // get_exe_path
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <linmath.h>	// mat4x4

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl_utility.h"	// gl_check_error

glfw_renderer::glfw_renderer(glfw_window * w) : renderer(w) {
	// NOTE: OpenGL error checks have been omitted for brevity
	if (0 == gladLoadGL(glfwGetProcAddress)) {
		throw exception("gladLoadGL Error");
	};
	glfwSwapInterval(1);
	
	m_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.vert.shad")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.frag.shad")).string().c_str()
	);
	m_vertex.init();
}

glfw_renderer::~glfw_renderer() {
	//glfw_DestroyRenderer(m_glfw_component);
	if (auto w = dynamic_cast<glfw_window*>(m_window)) {
		glfwSetWindowShouldClose(w->get_adaptee(), GLFW_TRUE);
	}
}

// overrided methods
void glfw_renderer::render() {
	/* Render here */
	gl_check_error(__FILE__, __LINE__);
	if (auto w = dynamic_cast<glfw_window*>(m_window)) {
		int width, height;
		glfwGetFramebufferSize(w->get_adaptee(), &width, &height);
		const float ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// petrolio
		//glClearColor(0.86f, 0.86f, 0.86f, 1.0f);	// gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gl_check_error(__FILE__, __LINE__);

		// use programs previously loaded
		m_shader.use();
		// use vertexes previously loaded
		m_vertex.use();

		for (const auto& d : m_drawables) {		
			this->draw(d->get_texture());
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(w->get_adaptee());
		gl_check_error(__FILE__, __LINE__);
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
	if (auto texture = dynamic_cast<glfw_texture*>(t)) {
		// use programs previously loaded
		m_shader.use();
		// use vertexes previously loaded
		m_vertex.use();

		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(t->get_where().get_x(), t->get_where().get_y(), 0.0f));
		transform = glm::rotate(transform,t->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(*m_shader.get_adaptee(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		gl_check_error(__FILE__, __LINE__);

		auto pos = texture->get_where();
		glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
		gl_check_error(__FILE__, __LINE__);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		gl_check_error(__FILE__, __LINE__);
	}
	else {
		tsg::print("Error casting texture in glfw_texture");
		throw;
	}
	/*
	if (auto texture = dynamic_cast<glfw_texture*>(t)) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
		glDisable(GL_TEXTURE_2D);
	}
	*/
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