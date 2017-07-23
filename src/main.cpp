#include "shader.h"
#include "globals.h"
#include "glfw.h"
#include "water.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <chrono>
#include <memory>

static const char* project_name = "Ripples";

//static const GLfloat quad[12] = {
//	-0.5f, -0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	-0.5f, 0.5f, 0.0f,
//	0.5f, 0.5f, 0.0f
//};

float tex_coords[12] = {
	0.0, 1.0, // Top-left.
	0.0, 0.0, // Bottom-left.
    1.0, 1.0, // Top-right.
    
	
	1.0, 1.0, // Top-right.
    0.0, 0.0, // Bottom-left.
    1.0, 0.0 // Bottom-right.
};

struct rect {
    rect(float top, float bottom, float left, float right) :
    data{
        left, top, // Top-left.
        left, bottom, // Bottom-left.
        right, top, // Top-right.
        
        
        right, top, // Top-right.
        left, bottom, // Bottom-left.
        right, bottom // Bottom-right.
    } {
    }
    float data[12];
};

//static const GLfloat quad[12] = {
//    -0.5f, 0.5f, 0.0f,  // top left.
//    -0.5f, -0.5f, 0.0f, // bottom left.
//    0.5f, 0.5f, 0.0f,    // top right.
//    0.5f, -0.5f, 0.0f  // bottom right.
//};

//static const GLfloat quad[12] = {
//    -0.5f, 0.5f, // top left.
//    -0.5f, -0.5f, // bottom left.
//    0.5f, 0.5f, // top right.
//    
//    0.5f, 0.5f, // top right.
//    -0.5f, -0.5f, // bottom left.
//    0.5f, -0.5f // bottom right.
//};

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
		model_location = glGetUniformLocation(program, "M");
		vpos_location = glGetAttribLocation(program, "vPos");
//		vcol_location = glGetAttribLocation(program, "vCol");
		vuv_location = glGetAttribLocation(program, "water_uv");

		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

        
        
        
        
        
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        std::vector<rect> rects;
        std::vector<rect> text_rects;
        //
        
        for(int j = 0; j < 100; j++) {
            float p0 = j / 100.0f;
            float p1 = (j + 1) / 100.0f;
            for(int i = 0; i < 100; i++) {
                float k0 = i / 100.0f;
                float k1 = (i + 1) / 100.0f;
                rects.push_back(rect(-0.5f + p0, -0.5 + p1, -0.5 + k0, -0.5 + k1));
                text_rects.push_back(rect(p0, p1, k0, k1));
            }
        }
        
		glBufferData(GL_ARRAY_BUFFER, rects.size() * 12 * sizeof(float), &rects[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
				0, (void*) 0);

//		glEnableVertexAttribArray(vcol_location);
//		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
//				0, (void*) (sizeof(float) * 3));

		glGenBuffers(1, &uv_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        
        
        
        
		glBufferData(GL_ARRAY_BUFFER, text_rects.size() * 12 * sizeof(float), &text_rects[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(vuv_location);
		glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
		GL_CHECK_ERROR();

	}

	~Opengl() {

	}

	GLuint vertex_array, vertex_buffer, uv_buffer, vertex_shader, fragment_shader, program;
	GLint vp_location, vpos_location, vcol_location, model_location, vuv_location;
};

int main(int, char**) {
	printf("%s\n", project_name);
	Glfw glfw{ project_name };
	Opengl opengl;
	Camera camera;
	Water water(1024, 1024);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

	auto new_frame_t = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(glfw.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration
				= new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		camera.update();
		water.update(dt);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_TEXTURE_2D);

		glDisable(GL_CULL_FACE);
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);

		glUseProgram(opengl.program);

		glUniformMatrix4fv(opengl.vp_location, 1, GL_FALSE, &camera.vp[0][0]);
		glUniformMatrix4fv(opengl.model_location, 1, GL_FALSE, &model[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(opengl.vertex_array);
		glBindTexture(GL_TEXTURE_2D, water.texture_id);

		glDrawArrays(GL_TRIANGLES, 0, 6 * 100 * 100);
		glBindVertexArray(0);

		glfw.post_render();
	}

	return 0;
}
