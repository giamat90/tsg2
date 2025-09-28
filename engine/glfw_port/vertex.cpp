#ifdef GLFW_GAME 

#include "vertex.h"
#include "gl_utility.h"

/* std includes */
#include <cassert>

/* sprite impl */
texture_vertex::texture_vertex() : vertex(), m_index_buffer(0u), m_vertex_buffer(0u) {}

texture_vertex::~texture_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);
	glDeleteVertexArrays(1, &m_adaptee);
}


void texture_vertex::init() {
	// texture - buffers and arrays
	glGenVertexArrays(1, &m_adaptee);
	gl_check_error(__FILE__, __LINE__);
	glBindVertexArray(m_adaptee);
	gl_check_error(__FILE__, __LINE__);

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
	gl_check_error(__FILE__, __LINE__);

	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indexes), m_indexes, GL_STATIC_DRAW);
	gl_check_error(__FILE__, __LINE__);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	gl_check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
	gl_check_error(__FILE__, __LINE__);
}

void texture_vertex::use() {
	glBindVertexArray(m_adaptee);
	gl_check_error(__FILE__, __LINE__);
}

/* mesh impl */
mesh_vertex::mesh_vertex() : vertex() {}
mesh_vertex::~mesh_vertex() {}
void mesh_vertex::init() {
	assert(0);
}
void mesh_vertex::use() {
	assert(0);
}
/* box2D impl */
box2D_vertex::box2D_vertex() : vertex(), m_index_buffer(0u), m_vertex_buffer(0u) {}

box2D_vertex::~box2D_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);
	glDeleteVertexArrays(1, &m_adaptee);
}

void box2D_vertex::init() {
	// texture - buffers and arrays
	glGenVertexArrays(1, &m_adaptee);
	gl_check_error(__FILE__, __LINE__);
	glBindVertexArray(m_adaptee);
	gl_check_error(__FILE__, __LINE__);
	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
	gl_check_error(__FILE__, __LINE__);
	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indexes), m_indexes, GL_STATIC_DRAW);
	gl_check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	gl_check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
	gl_check_error(__FILE__, __LINE__);
}

void box2D_vertex::use() {
	glBindVertexArray(m_adaptee);
	gl_check_error(__FILE__, __LINE__);
}

#endif