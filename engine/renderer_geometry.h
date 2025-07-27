#pragma once

#include "texture.h"
#include "geometry.h"

class drawable_bounding_volume : public texture_object {
public:
	drawable_bounding_volume(geometry::bounding_volume * bv = nullptr, const scalar scale = scalar(0)) : m_bv(bv) {
		assert(m_bv != nullptr);
		m_texture = texture::create_texture();
		m_texture->set_scale(scale);
		assert(m_texture != nullptr);
	}
	~drawable_bounding_volume() {
		if (m_texture) {
			delete m_texture;
		}
	}
	void init() override {};
	void update() {
		switch (m_bv->get_type())
		{
		case geometry::bounding_volume::type::box: {
			auto box = static_cast<geometry::box2D*>(m_bv);
			//m_texture->set_size(int(box->get_width()), int(box->get_height()));
			m_texture->set_where(texture::position({ box->get_center()[geometry::AXES::X], box->get_center()[geometry::AXES::Y] }));
			break;
		}
		default:
			assert(0); // unsupported bounding volume type
			break;
		}
	}
	void set_bounding_volume(geometry::bounding_volume * const bv) {
		assert(bv != nullptr);
		m_bv = bv;
	}
	geometry::bounding_volume * const get_bounding_volume() const { return m_bv; }
protected:
	geometry::bounding_volume* m_bv{ nullptr };
};