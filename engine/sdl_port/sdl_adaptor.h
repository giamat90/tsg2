#pragma once

#include "../tsg2.h"

template <typename T>
class TSG2_API sdl_adaptor {
public:
	sdl_adaptor() {}
	explicit sdl_adaptor(const T* c) { m_sdl_component = c; }
	sdl_adaptor& operator=(const T* c) { m_sdl_component = c; }
	T* const get_sdl_component() { return m_sdl_component; }
	void set_sdl_component(T * const c) { m_sdl_component = c; }
protected:
	T* m_sdl_component{ nullptr };
};
