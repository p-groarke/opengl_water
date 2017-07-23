#pragma once

#include "gl/gl.hpp"
#include <fst/print.h>
#include <string>

namespace gl {
void print_if_error() {
#define CASE_PRINT_GL_ERR_CODE(e)                                              \
  case e:                                                                      \
    fst::errprint(#e);                                                         \
    break
  GLenum err = glGetError();
  switch (err) {
    CASE_PRINT_GL_ERR_CODE(GL_INVALID_ENUM);
    CASE_PRINT_GL_ERR_CODE(GL_INVALID_VALUE);
    CASE_PRINT_GL_ERR_CODE(GL_INVALID_OPERATION);
    CASE_PRINT_GL_ERR_CODE(GL_OUT_OF_MEMORY);
    CASE_PRINT_GL_ERR_CODE(GL_INVALID_FRAMEBUFFER_OPERATION);
    CASE_PRINT_GL_ERR_CODE(GL_INVALID_INDEX);
  case GL_NO_ERROR:
    return;
  default:
    fst::errprint("Unknown :", (int)err);
  }
#undef CASE_PRINT_GL_ERR_CODE
}
} // namespace gl
