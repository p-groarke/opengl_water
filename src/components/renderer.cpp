#include "components/renderer.h"
#include "engine/globals.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
static inline bool gl_program_was_linked(int id)
{
	int result;
	char info[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(id, 1024, nullptr, (char*)info);
		OUTPUT_ERROR("OpenGL initialization error : %s\n", info);
	}
	return result != 0;
}

static inline bool gl_shader_was_compiled(int id)
{
	int result;
	char info[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(id, 1024, nullptr, (char*)info);
		OUTPUT_ERROR("OpenGL initialization error : %s\n", info);
	}
	return result != 0;
}
} // namespace

void Renderer::set_shader_path(const std::string& path)
{
	_relative_path = path;
}

void Renderer::load_shader(GLenum shader_type, const std::string& filename)
{
	//	std::string path = app::executable_dir + _relative_path + filename;
	std::string path = app::ExecutableDirectory::path + _relative_path + filename;

	std::ifstream f(path, std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		OUTPUT_ERROR("Couldn't read shader file : %s", path.c_str());
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
		OUTPUT_MSG("In shader file : %s", path.c_str());
		std::exit(-1);
	}
}

void Renderer::create()
{
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

void Renderer::init(ts::Entity)
{
}

void Renderer::update(ts::Entity, float)
{
	if (glfwGetKey(Window::main->window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(Window::main->window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Renderer::render(ts::Entity, float)
{
	GL_CHECK_ERROR();
}

void Renderer::destroy(ts::Entity)
{
	glDeleteProgram(program);
}

Renderer::Shader& Renderer::get_shader(GLenum shader_type)
{
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
