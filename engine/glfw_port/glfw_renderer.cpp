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
		const float ratio = width / static_cast<float>(height);

		glViewport(0, 0, width, height);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// petrolio
		//glClearColor(0.86f, 0.86f, 0.86f, 1.0f);	// gray
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	/* TODO */
}

void glfw_renderer::set_draw_color(const color& c)
{
	/* TODO */
}

void glfw_renderer::draw(sprite* s) {
	/* TODO */
}

void glfw_renderer::draw(texture* t) {
	if (auto texture = dynamic_cast<glfw_texture*>(t)) {
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(t->get_where().get_x(), t->get_where().get_y(), 0.0f));
		transform = glm::rotate(transform,t->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(*m_shader.get_adaptee(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		gl_check_error(__FILE__, __LINE__);

		glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
		gl_check_error(__FILE__, __LINE__);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		gl_check_error(__FILE__, __LINE__);
	}
	else {
		tsg::print("Error casting texture in glfw_texture");
		throw;
	}
}

void glfw_renderer::draw(font* f) {
	/* TODO */
}

void glfw_renderer::draw(geometry::shape* r) {
	/* TODO */
	//glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//transform = glm::translate(transform, glm::vec3(r->get_center().get_x(), r->get_center().get_y(), 0.0f));
	//transform = glm::rotate(transform, r->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

	//unsigned int transformLoc = glGetUniformLocation(*m_shader.get_adaptee(), "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//gl_check_error(__FILE__, __LINE__);

	//glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
	//gl_check_error(__FILE__, __LINE__);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//gl_check_error(__FILE__, __LINE__);
}

void glfw_renderer::draw(const geometry::box3D& box) {		
	// use programs previously loaded
	m_shader.use();
	// use vertexes previously loaded
	m_vertex.use();
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(box.get_center().get_x(), box.get_center().get_y(), 0.0f));
	transform = glm::rotate(transform, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	unsigned int transformLoc = glGetUniformLocation(*m_shader.get_adaptee(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	gl_check_error(__FILE__, __LINE__);

	//glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
	gl_check_error(__FILE__, __LINE__);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	gl_check_error(__FILE__, __LINE__);

}