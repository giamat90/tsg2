#include "gl_utility.h"
#include <tsg/io.h>		// print
#include "gl_includes.h"

#ifdef _DEBUG
void gl_check_error(const std::string& f, int r) {
	if (auto err = glGetError()) {
		tsg::print("Error {} throwed in file {} row {}", err, f, r);
		throw err;
	}
};
#else
void gl_check_error(const std::string& f, int r) {};
#endif