#ifdef GLFW_GAME 

#include "gl_utility.h"
#include "gl_includes.h"

/* tsg includes */
#include <tsg/io.h>		// print
#include <tsg/logger.h> // logger

/* std includes */
#include <cassert>

#ifdef _DEBUG
void gl_check_error(const std::string& f, int r) {
	if (auto err = glGetError()) {
		tsg::print("OpenGL Error {} throwed in file {} row {}", err, f, r);
		tsg::logger::get_instance().write("OpenGL Error {} throwed in file {} row {}", err, f, r);
		assert(0);
	}
};
#else
void gl_check_error(const std::string& f, int r) { return; };
#endif

#endif