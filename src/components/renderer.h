#pragma once
#include "engine/component.h"
#include "engine/glfw.h"

#include <string>
#include <limits>

struct Renderer : public Component {
	struct Shader {
		Shader(GLenum _type)
			: type(_type)
			, handle(std::numeric_limits<unsigned>::max())
		{}

		bool invalid() {
			return handle == std::numeric_limits<unsigned>::max();
		}

		const GLenum type = 0;
		GLuint handle = 0;
	};

	void set_shader_path(const std::string& path);
	void load_shader(GLenum shader_type, const std::string& filename);
	void create();
	void init() override;
	void render(float dt) override;
	void destroy() override;

	GLuint program;

private:
	Renderer::Shader& get_shader(GLenum shader_type);

	static constexpr size_t _shader_count = 5;
	Shader _shaders[_shader_count] = {
		{ GL_VERTEX_SHADER }
		, { GL_TESS_CONTROL_SHADER }
		, { GL_TESS_EVALUATION_SHADER }
		, { GL_GEOMETRY_SHADER }
		, { GL_FRAGMENT_SHADER }
	};

	#define vertex_shader _shaders[0];
	#define tess_control_shader _shaders[1];
	#define tess_eval_shader _shaders[2];
	#define geometry_shader _shaders[3];
	#define fragment_shader _shaders[4];
	std::string _relative_path = "";
};
