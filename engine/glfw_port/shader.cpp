#include "shader.h"
#include <fstream>	// ifstream
#include <sstream>	// stringstream
#include <tsg/io.h>	// print
#include "gl_utility.h" // gl_check_error

shader::shader(const char* vertex_path, const char* fragment_path) {
	init(vertex_path, fragment_path);
}

void shader::init(const char* vertex_path, const char* fragment_path) {
	m_adaptee = glCreateProgram();
	GLuint vertex_id = load(vertex_path, SHADER_TYPE::VERTEX);
	GLuint fragment_id = load(fragment_path, SHADER_TYPE::FRAGMENT);
	glLinkProgram(m_adaptee);
	// check linking
	int success;
	glGetProgramiv(m_adaptee, GL_LINK_STATUS, &success);
	gl_check_error(__FILE__, __LINE__);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(m_adaptee, 512, NULL, info_log);
		tsg::print("Compiling shader {} fails with error: {}", m_adaptee, info_log);
	}
	// freeing memory
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

shader::~shader() {

}

GLuint shader::load(const char * path, const SHADER_TYPE type) {
	std::ifstream stream;
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open files
		stream.open(path);
		std::stringstream ss;
		// read file's buffer contents into streams
		ss << stream.rdbuf();
		// close file handlers
		stream.close();
		std::string code_string = ss.str();
		const char* code = code_string.c_str();

		// create and compile shader program
		GLuint local_shader = glCreateShader(type == SHADER_TYPE::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		glShaderSource(local_shader, 1, &code, NULL);	
		gl_check_error(__FILE__, __LINE__);
		glCompileShader(local_shader);
		gl_check_error(__FILE__, __LINE__);
		// check compilation
		int success;
		glGetShaderiv(local_shader, GL_COMPILE_STATUS, &success);
		gl_check_error(__FILE__, __LINE__);
		if (!success) {
			char info_log[512];
			glGetShaderInfoLog(local_shader, 512, NULL, info_log);
			tsg::print("Compiling shader {} fails with error: {}", local_shader, info_log);
		}
		// attach to main program
		glAttachShader(m_adaptee, local_shader);
		gl_check_error(__FILE__, __LINE__);
		return local_shader;
	}
	catch (std::fstream::failure& e) {
		tsg::print("Shader loading {} exception {}", path, e.what());
		throw;
	}
	catch (...) {
		tsg::print("Shader loading {} exception unknown", path);
		throw;
	}
}

void shader::use() {
	glUseProgram(m_adaptee);
	gl_check_error(__FILE__, __LINE__);
}