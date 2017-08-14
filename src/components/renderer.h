#pragma once
#define NOMINMAX
#include "engine/component.h"
#include "engine/window.h"

#include <string>
#include <limits>

static char _err[64] = "";
#define GL_CHECK_ERROR()	{ \
	gl_error_string(_err); \
	if (strlen(_err) != 0) { \
		OUTPUT_ERROR("OpenGL error : %s", _err); \
		memset(_err, 0, sizeof(_err)); \
	} \
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
	void update(float dt) override;
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
