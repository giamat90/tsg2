#include "vertex.h"
#include "gl_utility.h"

sprite_vertex::sprite_vertex() : vertex(), m_index_buffer(0u), m_vertex_buffer(0u) {}

sprite_vertex::~sprite_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);
	glDeleteVertexArrays(1, &m_adaptee);
}


void sprite_vertex::init() {
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

void sprite_vertex::use() {
	glBindVertexArray(m_adaptee);
	gl_check_error(__FILE__, __LINE__);
}