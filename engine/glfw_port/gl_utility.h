#ifndef GL_UTILITY_H
#define GL_UTILITY_H

#include <string>

void gl_check_error(const std::string& f = __FILE__, int r = __LINE__);

#endif