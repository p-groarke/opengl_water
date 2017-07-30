#pragma once
#include <glad/glad.h>
#include <cstdio>
#include <cstring>

static char _err[64] = "";
#define GL_CHECK_ERROR()	{ \
	gl_error_string(_err); \
	if (strlen(_err) != 0) { \
		OUTPUT_ERROR("OpenGL error : %s", _err); \
		memset(_err, 0, sizeof(_err)); \
	} \
}

static inline bool gl_program_was_linked(int id) {
	int result;
	char info[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(id, 1024, nullptr, (char *)info);
		OUTPUT_ERROR("OpenGL initialization error : %s\n", info);
	}
	return result != 0;
}

static inline bool gl_shader_was_compiled(int id) {
	int result;
	char info[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 1024, nullptr, (char *)info);
		OUTPUT_ERROR("OpenGL initialization error : %s\n", info);
	}
	return result != 0;
}

static inline const char* _gl_get_err_string(GLenum type) {
	switch (type) {
		case GL_NO_ERROR: return "GL_NO_ERROR ";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM ";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE ";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION ";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION ";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY ";
//		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW "; // Until OpenGL 4.2
//		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW "; // Until OpenGL 4.2
		default: return "invalid error number ";
	}
}

static inline void gl_error_string(char* msg) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
#ifdef _MSC_VER
		strcat_s(msg, 256, _gl_get_err_string(err));
#else
		strcat(msg, _gl_get_err_string(err));
#endif
	}
}

