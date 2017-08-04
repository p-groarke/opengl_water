#include "components/renderer.h"
#include "engine/globals.h"
#include "engine/opengl.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
} // namespace anonymous

void Renderer::set_shader_path(const std::string& path) {
	_relative_path = path;
}

void Renderer::load_shader(GLenum shader_type, const std::string& filename) {
	std::ifstream f(_relative_path + filename, std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		OUTPUT_ERROR("Couldn't read shader file : %s%s"
				, _relative_path.c_str(), filename.c_str());
		std::exit(-1);
	}

	size_t size = f.tellg();
	f.seekg(std::ios::beg);

	std::string buffer;
	buffer.resize(size);
	f.read(&buffer[0], size);

	Shader& ret = get_shader(shader_type);
	ret.handle = glCreateShader(shader_type);

	const char* src = buffer.c_str();
	glShaderSource(ret.handle, 1, &src, NULL);
	glCompileShader(ret.handle);

	if (!gl_shader_was_compiled(ret.handle)) {
		OUTPUT_MSG("In shader file : %s%s"
				, _relative_path.c_str(), filename.c_str());
		std::exit(-1);
	}
}

void Renderer::create() {
	program = glCreateProgram();
	for (size_t i = 0; i < _shader_count; ++i) {
		if (_shaders[i].invalid())
			continue;
		glAttachShader(program, _shaders[i].handle);
	}
	glLinkProgram(program);

	GL_CHECK_ERROR();

	bool was_init = true;
	for (size_t i = 0; i < _shader_count; ++i) {
		if (_shaders[i].invalid())
			continue;
		was_init &= gl_shader_was_compiled(_shaders[i].handle);
	}

	was_init &= gl_program_was_linked(program);

	if (!was_init)
		std::exit(-1);
}

void Renderer::init() {

}

void Renderer::render(float) {
	GL_CHECK_ERROR();
}

void Renderer::destroy() {
	glDeleteProgram(program);
}

Renderer::Shader& Renderer::get_shader(GLenum shader_type) {
	switch (shader_type) {
		// OpenGL 4.3 fuck Apple
//		case GL_COMPUTE_SHADER: {
//			return compute_shader;
//		} break;
		case GL_VERTEX_SHADER: {
			return vertex_shader;
		} break;
		case GL_TESS_CONTROL_SHADER: {
			return tess_control_shader;
		} break;
		case GL_TESS_EVALUATION_SHADER: {
			return tess_eval_shader;
		} break;
		case GL_GEOMETRY_SHADER: {
			return geometry_shader;
		} break;
		case GL_FRAGMENT_SHADER: {
			return fragment_shader;
		} break;
	}
	OUTPUT_ERROR("Invalid shader type %u", shader_type);
	std::exit(-1);
	return vertex_shader;
}
