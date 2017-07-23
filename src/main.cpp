#include "shader.h"
#include "globals.h"
#include "glfw.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <memory>

static const char* project_name = "Ripples";

static const GLfloat quad[12] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

struct Camera {

	void update() {
		glm::vec3 r_axis{0.f, 1.f, 0.f};
		glm::quat quat = glm::angleAxis(glm::radians(2.f)
				, r_axis);
		position = quat * position;

		view = glm::lookAt(position, glm::vec3(0.f, 0.f, 0.f)
				, glm::vec3(0.f, 1.f, 0.f));
		projection = glm::perspective(glm::radians(fov)
				, Glfw::window_ratio, 0.1f, 100.0f);
		vp = projection * view;
	}

	const float fov = 90.f;
	glm::vec3 position{3.f, 2.f, 2.f};
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 vp;
};

struct Opengl {
	Opengl() {
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		GL_CHECK_ERROR();

		bool was_init = true;
		was_init &= gl_shader_was_compiled(vertex_shader);
		was_init &= gl_shader_was_compiled(fragment_shader);
		was_init &= gl_program_was_linked(program);
		if (!was_init)
			std::exit(-1);

		vp_location = glGetUniformLocation(program, "VP");
		vpos_location = glGetAttribLocation(program, "vPos");
		vcol_location = glGetAttribLocation(program, "vCol");

		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
				0, (void*) 0);

		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
				0, (void*) (sizeof(float) * 3));

		glBindVertexArray(0);
		GL_CHECK_ERROR();
	}

	~Opengl() {

	}

	GLuint vertex_array, vertex_buffer, vertex_shader, fragment_shader, program;
	GLint vp_location, vpos_location, vcol_location;
};

int main(int, char**) {
	printf("%s\n", project_name);
	Glfw glfw{ project_name };
	Opengl opengl;
	Camera camera;

	while (!glfwWindowShouldClose(glfw.window)) {
		camera.update();

		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(opengl.program);
		glUniformMatrix4fv(opengl.vp_location, 1, GL_FALSE, &camera.vp[0][0]);
		glBindVertexArray(opengl.vertex_array);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glfw.post_render();
	}

	return 0;
}
