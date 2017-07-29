#include "shader.h"
#include "globals.h"
#include "glfw.h"
#include "water.hpp"

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <chrono>
#include <memory>

static const char* project_name = "Ripples";

static const GLfloat quad[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
//	-0.5f, 0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	0.5f, 0.5f, 0.0f
};

float tex_coords[8] = {
	0.0, 1.0, // Top-left.
	0.0, 0.0, // Bottom-left.
	1.0, 0.0, // Bottom-right.
	1.0, 1.0 // Top-right.
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

		tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tess_control_shader, 1, &tess_control_shader_source, NULL);
		glCompileShader(tess_control_shader);

		tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tess_eval_shader, 1, &tess_eval_shader_source, NULL);
		glCompileShader(tess_eval_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, tess_control_shader);
		glAttachShader(program, tess_eval_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		GL_CHECK_ERROR();

		bool was_init = true;
		was_init &= gl_shader_was_compiled(vertex_shader);
		was_init &= gl_shader_was_compiled(tess_control_shader);
		was_init &= gl_shader_was_compiled(tess_eval_shader);
		was_init &= gl_shader_was_compiled(fragment_shader);
		was_init &= gl_program_was_linked(program);
		if (!was_init)
			std::exit(-1);

		vp_location = glGetUniformLocation(program, "VP");
		model_location = glGetUniformLocation(program, "M");
//		tlvl_inner_loc = glGetUniformLocation(program, "TessLevelInner");
//		tlvl_outer_loc = glGetUniformLocation(program, "TessLevelOuter");


		vpos_location = glGetAttribLocation(program, "vPos");
//		vcol_location = glGetAttribLocation(program, "vCol");
		vuv_location = glGetAttribLocation(program, "water_uv");

		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
				0, (void*) 0);

//		glEnableVertexAttribArray(vcol_location);
//		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
//				0, (void*) (sizeof(float) * 3));

		glGenBuffers(1, &uv_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(vuv_location);
		glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
		GL_CHECK_ERROR();

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);
//		glEnable(GL_TEXTURE_2D);

		glDisable(GL_CULL_FACE);
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);

//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(program);

		GL_CHECK_ERROR();
	}

	~Opengl() {
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteProgram(program);
	}

	GLuint vertex_array, vertex_buffer, uv_buffer, vertex_shader, fragment_shader
		, tess_control_shader, tess_eval_shader, program;
	GLint vp_location, vpos_location, vcol_location, model_location, vuv_location
		, tlvl_inner_loc, tlvl_outer_loc;
};

int main(int, char**) {
	printf("%s\n", project_name);
	Glfw glfw{ project_name };
	Opengl opengl;
	Camera camera;
	Water water(1024, 1024);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
//	glm::mat4 model = glm::mat4(1.0f);

	auto new_frame_t = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(glfw.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration
				= new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		camera.update();
		water.update(dt);

		GL_CHECK_ERROR();

//		glBindTexture(GL_TEXTURE_2D, water.texture_id);
		glBindVertexArray(opengl.vertex_array);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glUniformMatrix4fv(opengl.vp_location, 1, GL_FALSE, &camera.vp[0][0]);
		glUniformMatrix4fv(opengl.model_location, 1, GL_FALSE, &model[0][0]);
//		printf("%s\n", glm::to_string(camera.vp).c_str());
//		glUniform1f(opengl.tlvl_inner_loc, 4);
//		glUniform1f(opengl.tlvl_outer_loc, 2);

//		glActiveTexture(GL_TEXTURE0);

//		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_PATCHES, 0, 4);
//		glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfw.post_render();
//		glClearColor(0.5, 0.5, 1.f, 1.f);
		GL_CHECK_ERROR();
	}

	return 0;
}
