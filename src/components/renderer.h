#pragma once
#include "engine/component.h"
#include "engine/globals.h"

#include <fstream>
#include <string>
#include <vector>


struct Renderer : public Component {
	struct Shader {
		GLenum type;
		GLuint handle;
	};

	void set_shader_path(const std::string& path) {
		_relative_path = path;
	}

	void load_vertex_shader(const std::string& filename) {
		vertex_shader = load_shader_src(GL_VERTEX_SHADER, filename);
	}

	void init() {

	}

	GLuint program;
	Shader vertex_shader;
	Shader tess_control_shader;
	Shader tess_eval_shader;
	Shader geometry_shader;
	Shader fragment_shader;

private:
	Shader load_shader_src(GLenum type, const std::string& filename) {
		std::ifstream f(_relative_path + filename);
		if (!f.is_open()) {
			OUTPUT_ERROR("Couldn't read shader file : %s/%s"
					, _relative_path.c_str(), filename.c_str());
			std::exit(-1);
		}

		f.seekg(0, std::ios::end);
		size_t size = f.tellg();
		std::string buffer;
		buffer.reserve(size);
		f.seekg(0);
		f.read(&buffer[0], size);

		Shader ret;
		ret.handle = glCreateShader(type);
		const char* src = buffer.c_str();
		glShaderSource(ret.handle, 1, &src, NULL);
		glCompileShader(ret.handle);
		if (!gl_shader_was_compiled(ret.handle)) {
			std::exit(-1);
		}

		return ret;
	}

	std::string _relative_path = "";
};
