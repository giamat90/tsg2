#include "field.h"

field::field() : static_object() {
	m_texture = texture::create_texture();
	m_texture->set_where({ 0, 0 });
}

field::~field() {
	delete m_texture;
}