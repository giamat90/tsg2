#include "glfw_renderer.h"
#include "glfw_texture.h"
#include "glfw_font.h"
#include <geometry.h>
#include <tsg/io.h>	// print
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <linmath.h>	// mat4x4

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class gl_attributes {
	friend glfw_renderer;
	using gl_program = GLuint;
	typedef struct Vertex
	{
		vec2 pos;
		vec3 col;
	} Vertex;
private:
	gl_program program;
	GLint mvp_location;
	GLint vpos_location;
	GLint vcol_location;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
private:
	//const Vertex vertices[3] =
	//{
	//	{ { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
	//	{ {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
	//	{ {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
	//};
	//const float vertices[9] = {
	//	-.5f, -.5f,.0f,
	//	.5f, -.5f,.0f,
	//	.0f, .5f,.0f,
	//};
	float vertices[32] = {
	0.5f, 0.5f, 0.f, 1.0f, 0.f, 0.0f, 1.0f, 1.0f, // top left
	0.5f, -0.5f, 0.f, 0.f, 1.0f, 0.0f, 1.0f, 0.f, // top right
	-0.5f,-0.5f, 0.f, 0.f, 0.f, 1.0f, 0.0f, 0.0f, // bottom right
	-0.5f, 0.5f, 0.f, 1.0f, 1.0f, 0.0f, 0.f, 1.f  // bottom left
	};

	const char* vertex_shader_text =
	"#version 330\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(aPos, 1.0);\n"
	"    ourColor = aColor;\n"
	"    TexCoord = aTexCoord;\n"
	"}\n";
#if ORIGINAL
	const char* vertex_shader_text =
	"#version 330\n"
	"uniform mat4 MVP;\n"
	"in vec3 vCol;\n"
	"in vec2 vPos;\n"
	"out vec3 color;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
	"    color = vCol;\n"
	"}\n";
#endif
	const char* fragment_shader_text =
	"#version 330\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"    FragColor = texture(ourTexture, TexCoord);\n"
	"}\n";
#if ORIGINAL
	const char* fragment_shader_text =
	"#version 330\n"
	"in vec3 color;\n"
	"out vec4 fragment;\n"
	"void main()\n"
	"{\n"
	"    fragment = vec4(color, 1.0);\n"
	"}\n";
#endif
#if 0
	"#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"       // Vertex position
	"layout(location = 1) in vec2 aTexCoord;\n"// Texture coordinate

	"uniform mat4 model;\n"      // Model matrix
	"uniform mat4 view;\n"       // View matrix
	"uniform mat4 projection;\n" // Projection matrix

	"out vec2 TexCoord;\n"       // Output to fragment shader

	"void main() {\n"
	// Transform vertex position to clip space
	"    gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
	// Pass texture coordinates to fragment shader
	"    TexCoord = aTexCoord;\n"
	"}\n";
	const char* fragment_shader_text =
	"#version 330 core\n"
	"in vec2 TexCoord;\n"           // Input from vertex shader
	"uniform sampler2D texture1;\n" // Texture sampler
	"out vec4 FragColor;\n"         // Output color
	"void main() {\n"
	// Sample texture and set as fragment color
	"    FragColor = texture(texture1, TexCoord);\n"
	"}\n";
#endif
};

class gl_texture_attributes {
	friend glfw_renderer;
	using gl_program = GLuint;
private:
	gl_program program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
private:
	/*/
	const float vertices[32] = {	// quad
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	/**/
	//
	const float vertices[24] = {	// triangle
		// positions          // colors           // texture coords
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	};
	/**/

	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	const char* vertex_shader_text =
		"#version 330\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"    ourColor = aColor;\n"
		"    TexCoord = aTexCoord;\n"
		"}\n";
	const char* fragment_shader_text =
		"#version 330\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"    FragColor = texture(ourTexture, TexCoord);\n"
		"}\n";
};

static gl_attributes s_gl_attr;
static gl_texture_attributes s_gl_texture_attr;

glfw_renderer::glfw_renderer(glfw_window * w) : renderer(w) {
	// NOTE: OpenGL error checks have been omitted for brevity
	if (0 == gladLoadGL(glfwGetProcAddress)) {
		throw exception("gladLoadGL Error");
	};

	glfwSwapInterval(1);

#if 0
	auto check_compilation = [&](GLuint shader) -> void {
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info_log[512];
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			tsg::print("Compiling shader {} fails with error: {}", shader, info_log);
		}
	};

	auto check_linking = [&](GLuint program) -> void {
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		tsg::print(glGetError());
		if (!success) {
			char info_log[512];
			glGetProgramInfoLog(program, 512, NULL, info_log);
			tsg::print("Compiling shader {} fails with error: {}", program, info_log);
		}
	};
		
	// texture - shaders
	s_gl_texture_attr.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(s_gl_texture_attr.vertex_shader, 1, &s_gl_texture_attr.vertex_shader_text, NULL);
	glCompileShader(s_gl_texture_attr.vertex_shader);
	tsg::print(glGetError());
	check_compilation(s_gl_texture_attr.vertex_shader);

	s_gl_texture_attr.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(s_gl_texture_attr.fragment_shader, 1, &s_gl_texture_attr.fragment_shader_text, NULL);
	glCompileShader(s_gl_texture_attr.fragment_shader);
	check_compilation(s_gl_texture_attr.fragment_shader);

	s_gl_texture_attr.program = glCreateProgram();
	glAttachShader(s_gl_texture_attr.program, s_gl_texture_attr.vertex_shader);
	glAttachShader(s_gl_texture_attr.program, s_gl_texture_attr.fragment_shader);
	glLinkProgram(s_gl_texture_attr.program);
	check_linking(s_gl_attr.program);
	tsg::print(glGetError());

	glDeleteShader(s_gl_texture_attr.vertex_shader);
	glDeleteShader(s_gl_texture_attr.fragment_shader);
#endif

	//m_s.init("C:\\tsg2\\test\\openGL_tests\\shaders\\texture_vert.shad", "C:\\tsg2\\test\\openGL_tests\\shaders\\texture_frag.shad");
	m_s.init("C:\\tsg2\\test\\openGL_tests\\shaders\\texture_transform_vert.shad", "C:\\tsg2\\test\\openGL_tests\\shaders\\texture_transform_frag.shad");

	// texture - buffers and arrays
	glGenVertexArrays(1, &s_gl_texture_attr.vertex_array);
	glBindVertexArray(s_gl_texture_attr.vertex_array);
	tsg::print(glGetError());

	glGenBuffers(1, &s_gl_texture_attr.vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_gl_texture_attr.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_gl_texture_attr.vertices), s_gl_texture_attr.vertices, GL_STATIC_DRAW);
	tsg::print(glGetError());


	glGenBuffers(1, &s_gl_texture_attr.element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_gl_texture_attr.element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_gl_texture_attr.indices), s_gl_texture_attr.indices, GL_STATIC_DRAW);
	tsg::print(glGetError());

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	tsg::print(glGetError());
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	tsg::print(glGetError());
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	tsg::print(glGetError());
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
	tsg::print(glGetError());
	if (auto w = dynamic_cast<glfw_window*>(m_window)) {
		int width, height;
		glfwGetFramebufferSize(w->get_adaptee(), &width, &height);
		const float ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tsg::print(glGetError());


		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		m_s.use();
		unsigned int transformLoc = glGetUniformLocation(*m_s.get_adaptee(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		tsg::print(glGetError());	

		for (const auto& d : m_drawables) {			
			if (auto texture = dynamic_cast<glfw_texture*>(d->get_texture())) {
				glActiveTexture(GL_TEXTURE0);
				tsg::print(glGetError());
				//glEnable(GL_TEXTURE_2D);
				//tsg::print(glGetError());
				glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
				tsg::print(glGetError());
				//GLint textureLocation = glGetUniformLocation(s_gl_attr.program, "myTexture");
				//glUniform1i(textureLocation, 0);
				//glUniform1i(glGetUniformLocation(s_gl_attr.vertex_shader, "fragment"), 0);
				tsg::print(glGetError());
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//glDisable(GL_TEXTURE_2D);
				tsg::print(glGetError());
			}
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(w->get_adaptee());
		tsg::print(glGetError());
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
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, *(texture->get_adaptee()));
		glDisable(GL_TEXTURE_2D);
	}
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