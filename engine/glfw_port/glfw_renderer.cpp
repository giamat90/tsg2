#ifdef GLFW_GAME 

#include "glfw_renderer.h"
#include "glfw_texture.h"
#include "glfw_sprite.h"
#ifdef TODO 
#include "glfw_mesh.h"
#endif
#include "glfw_font.h"
#include "gl_utility.h"	// gl_check_error
#include "geometry.h"

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/io.h>	// print

/* glm includes */
#include <linmath.h>	// mat4x4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using geometry::AXES;


glfw_renderer::glfw_renderer(glfw_window * w) : renderer<glfw_window, glfw_renderer>(w) {
	// NOTE: OpenGL error checks have been omitted for brevity
	if (0 == gladLoadGL(glfwGetProcAddress)) {
		throw exception("gladLoadGL Error");
	};
	glfwSwapInterval(1);

	m_texture_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.vert.shad")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.frag.shad")).string().c_str()
	);
	m_texture_vertex.init();
	
	m_sprite_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.vert.shad")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_transform.frag.shad")).string().c_str()
	);
	m_sprite_vertex.init();

	m_line_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line.vert.shad")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line.frag.shad")).string().c_str()
	);
	m_box2D_vertex.init();
}

glfw_renderer::~glfw_renderer() {
	//glfw_DestroyRenderer(m_glfw_component);
	glfwSetWindowShouldClose(m_window->get_adaptee(), GLFW_TRUE);
}

// overrided methods
void glfw_renderer::render() {
	/* Render here */
	gl_check_error(__FILE__, __LINE__);
	int width, height;
	glfwGetFramebufferSize(m_window->get_adaptee(), &width, &height);
	const float ratio = width / static_cast<float>(height);

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// petrolio
	//glClearColor(0.86f, 0.86f, 0.86f, 1.0f);	// gray
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_check_error(__FILE__, __LINE__);

	/* drawing bounding volumes */
	if (m_bv_obj.size() > 0) {
		// use programs previously loaded
		m_texture_shader.use();
		// use vertexes previously loaded
		m_texture_vertex.use();
		for (auto& bv : m_bv_obj) {
			bv->update();
			draw(bv->get_texture());
			//draw(bv->get_bounding_volume());
		}
	}
	/* draw textures */
	if (m_textures_obj.size() > 0) {
		// use programs previously loaded
		m_texture_shader.use();
		// use vertexes previously loaded
		m_texture_vertex.use();
		for (const auto& t : m_textures_obj) {
			this->draw(t->get_texture());
		}
	}
	/* drawing sprites */
	if (m_sprites_obj.size() > 0) {
		// use programs previously loaded
		m_sprite_shader.use();
		// use vertexes previously loaded
		m_sprite_vertex.use();
		for (const auto& s : m_sprites_obj) {
			this->draw(s->get_sprite());
		}
	}
	/* drawing meshes */
	if (m_meshes_obj.size() > 0) {
		/* TODO */
		assert(0);
		// use programs previously loaded
		m_mesh_shader.use();
		// use vertexes previously loaded
		m_mesh_vertex.use();
		for (const auto& m : m_meshes_obj) {
			this->draw(m->get_mesh());
		}
	}
	/* Swap front and back buffers */
	glfwSwapBuffers(m_window->get_adaptee());
	gl_check_error(__FILE__, __LINE__);
}

void glfw_renderer::clear() {
	/* TODO */
	assert(0);
}

void glfw_renderer::set_draw_color(const color& c)
{
	/* TODO */
	assert(0);
}


void glfw_renderer::draw(texture* t) {
	if (auto texture = dynamic_cast<glfw_texture*>(t)) {
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::scale(transform, glm::vec3(texture->get_scale(), texture->get_scale(), 1.0f));
		transform = glm::translate(transform, glm::vec3(texture->get_where()[AXES::X], texture->get_where()[AXES::Y], texture->get_where()[AXES::Z]));
		transform = glm::rotate(transform, texture->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		if (glIsTexture(*(texture->get_adaptee())) != GL_TRUE) {
			assert(0); // texture is not a valid OpenGL texture
		}

		unsigned int transformLoc = glGetUniformLocation(*m_texture_shader.get_adaptee(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		gl_check_error(__FILE__, __LINE__);

		glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
		gl_check_error(__FILE__, __LINE__);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		gl_check_error(__FILE__, __LINE__);
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
}

void glfw_renderer::draw(sprite* s) {
	if (auto sprite = dynamic_cast<glfw_sprite*>(s)) {
		assert(glIsTexture(*(sprite->get_adaptee())));

		sprite->set_active();

		GLint boundTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
		assert(boundTexture == *(sprite->get_adaptee()));

		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::scale(transform, glm::vec3(sprite->get_scale(), sprite->get_scale(), 1.0f));
		transform = glm::translate(transform, glm::vec3(sprite->get_where()[AXES::X], sprite->get_where()[AXES::Y], sprite->get_where()[AXES::Z]));
		transform = glm::rotate(transform, sprite->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(*m_sprite_shader.get_adaptee(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		gl_check_error(__FILE__, __LINE__);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		gl_check_error(__FILE__, __LINE__);
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
}

void glfw_renderer::draw(mesh* m) {
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(font* f) {
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(geometry::bounding_volume* bv) {
	switch (bv->get_type())
	{
	case geometry::bounding_volume::type::aabb:
	case geometry::bounding_volume::type::obb:
		if(bv->get_dimension() == 3) {
			draw(static_cast<geometry::box3D&>(*bv));
		}
		else if(bv->get_dimension() == 2) {
			draw(static_cast<geometry::box2D&>(*bv));
		}
		else {
			assert(0); // unsupported dimension
		}
		break;
	case geometry::bounding_volume::type::sphere:
		assert(0);
		break;
	default:
		break;
	}
}

/* TODO */
void glfw_renderer::draw(const geometry::box3D& box) {
	assert(0);
}

void glfw_renderer::draw(const geometry::box2D& box) {
#if 0
	m_sprite_vertex.use();

	auto vertices = bbox.getWireframeVertices();
	auto indices = bbox.getLineIndices();

	// Bind VAO
	glBindVertexArray(VAO);

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
		vertices.data(), GL_DYNAMIC_DRAW);

	// Upload index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_DYNAMIC_DRAW);

	// Set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set matrices
	int modelLoc = glGetUniformLocation(m_line_shader, "model");
	int viewLoc = glGetUniformLocation(m_line_shader, "view");
	int projLoc = glGetUniformLocation(m_line_shader, "projection");
	int colorLoc = glGetUniformLocation(m_line_shader, "color");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	// Render
	glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
#endif

#if 1
	GLuint VAO, VBO, EBO;
	GLuint box_texture;

	glGenTextures(1, &box_texture);
	gl_check_error(__FILE__, __LINE__);

	glBindTexture(GL_TEXTURE_2D, box_texture);
	gl_check_error(__FILE__, __LINE__);
	
	unsigned char whitePixel[] = { 255, 255, 255, 255 }; // RGBA
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
	gl_check_error(__FILE__, __LINE__);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl_check_error(__FILE__, __LINE__);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl_check_error(__FILE__, __LINE__);

	// Setup VAO, VBO, EBO...
	glGenVertexArrays(1, &VAO);
	gl_check_error(__FILE__, __LINE__);

	glGenBuffers(1, &VBO);
	gl_check_error(__FILE__, __LINE__);

	glGenBuffers(1, &EBO);
	gl_check_error(__FILE__, __LINE__);

	// Create line vertices with texture coordinates
	float vertices[16] = {
		// Positions								// Texture Coords
		box.get_min(AXES::X), box.get_min(AXES::Y), 0.0f, 0.0f, // Bottom-left
		box.get_max(AXES::X), box.get_min(AXES::Y), 1.0f, 0.0f, // Bottom-right
		box.get_max(AXES::X), box.get_max(AXES::Y), 1.0f, 1.0f, // Top-right
		box.get_min(AXES::X), box.get_max(AXES::Y), 0.0f, 1.0f  // Top-left
	};

	unsigned int indices[8] = {
		0, 1,  // Bottom edge
		1, 2,  // Right edge
		2, 3,  // Top edge
		3, 0   // Left edge
	};

	glBindVertexArray(VAO);
	gl_check_error(__FILE__, __LINE__);

	// Upload data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	gl_check_error(__FILE__, __LINE__);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gl_check_error(__FILE__, __LINE__);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	gl_check_error(__FILE__, __LINE__);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	gl_check_error(__FILE__, __LINE__);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_check_error(__FILE__, __LINE__);

	// use programs previously loaded
	m_texture_shader.use();
	// use vertexes previously loaded
	glActiveTexture(GL_TEXTURE0);
	gl_check_error(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, box_texture);
	gl_check_error(__FILE__, __LINE__);

	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//transform = glm::scale(transform, glm::vec3(t->get_scale(), t->get_scale(), 1.0f));
	transform = glm::translate(transform, glm::vec3(box.get_center()[AXES::X], box.get_center()[AXES::Y], 0.0f));
	//transform = glm::rotate(transform, t->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

	unsigned int transformLoc = glGetUniformLocation(*m_texture_shader.get_adaptee(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	gl_check_error(__FILE__, __LINE__);

	// Set line width
	glLineWidth(2.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	gl_check_error(__FILE__, __LINE__);

	glBindVertexArray(0);
	gl_check_error(__FILE__, __LINE__);

	glfwSwapBuffers(m_window->get_adaptee());
	gl_check_error(__FILE__, __LINE__);
#endif
#if 0
	// Create a quad with your existing vertex format
	float vertices[] = {
		// Positions								// Texture Coords
		box.get_min(AXES::X), box.get_min(AXES::Y), 0.0f, 0.0f, // Bottom-left
		box.get_max(AXES::X), box.get_min(AXES::Y), 1.0f, 0.0f, // Bottom-right
		box.get_max(AXES::X), box.get_max(AXES::Y), 1.0f, 1.0f, // Top-right
		box.get_min(AXES::X), box.get_max(AXES::Y), 0.0f, 1.0f  // Top-left
	};

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 }; // Triangle indices

	// Setup buffers as usual...

	// Switch to wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Use your existing shader and render as triangles
	m_texture_shader.use();
	// ... set uniforms, bind texture (even though it won't be visible much)

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Switch back to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
};

void glfw_renderer::draw(const drawable_bounding_volume&) {
	assert(0);
}

#endif