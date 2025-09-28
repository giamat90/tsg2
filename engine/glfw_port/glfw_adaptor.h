#pragma once

#include "tsg2.h"

template <typename T>
class TSG2_API glfw_adaptor {
public:
	glfw_adaptor() = default;
	glfw_adaptor(const T* c) : m_glfw_component(c) {};
	~glfw_adaptor() = default;
	inline glfw_adaptor& operator=(const T* c) { m_glfw_component = c; }
	inline T* const get_glfw_component() const { return m_glfw_component; }
	inline void set_glfw_component(T * const c) { m_glfw_component = c; }
protected:
	T* m_glfw_component{ nullptr };
};
