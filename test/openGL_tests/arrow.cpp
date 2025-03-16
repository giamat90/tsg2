#include "arrow.h"
#include <texture.h>

arrow::arrow() {
	m_texture = texture::create_texture();
	m_texture->load("./assets/arrow.png");
}

void arrow::update(const float delta_time) {
	/* ToDo */
}