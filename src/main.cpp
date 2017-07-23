#include "shader.h"
#include "globals.h"
#include "glfw.h"
#include "water.hpp"
#include "gl/vertex_array.hpp"
#include "gl/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <chrono>
#include <memory>

static const char* project_name = "Ripples";

struct rect {
    typedef float value_type;

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
        _vertex_shader = gl::shader(gl::shader::type::vertex, std::string(vertex_shader_source));
        _fragment_shader = gl::shader(gl::shader::type::fragment, std::string(fragment_shader_source));

        if(!_vertex_shader.compile()) {
            printf("%s\n", _vertex_shader.get_error_string().c_str());
            std::exit(-1);
        }
        
        if(!_fragment_shader.compile()) {
            printf("%s\n", _fragment_shader.get_error_string().c_str());
            std::exit(-1);
        }
        
        _program.attach_shader(_vertex_shader);
        _program.attach_shader(_fragment_shader);
        
        if(!_program.link()) {
            printf("%s\n", _program.get_error_string().c_str());
            std::exit(-1);
        }
        
        printf("TEST\n");

		GL_CHECK_ERROR();
        
        _program.activate();
        vp_location = _program.get_uniform_location("VP");
        model_location = _program.get_uniform_location("M");
        vpos_location = _program.get_attribute_location("vPos");
        // vcol_location = glGetAttribLocation(program, "vCol");
        vuv_location = _program.get_attribute_location("water_uv");
        
        printf("loc %d\n", vpos_location);
        
        std::vector<rect> rects;
        std::vector<rect> text_rects;
        
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
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        _water_plane.set_buffer(vpos_location, rects, 2, 6 * 100 * 100);
        _water_plane.set_buffer(vuv_location, text_rects, 2, 6 * 100 * 100);
        _water_plane.set_draw_type(gl::draw_type::triangles);

		GL_CHECK_ERROR();

	}

	~Opengl() {

	}

	GLint vp_location, vpos_location, vcol_location, model_location, vuv_location;
    gl::vertex_array<3> _water_plane;
    
    gl::shader _vertex_shader;
    gl::shader _fragment_shader;
    gl::program _program;
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

        opengl._program.activate();

		glUniformMatrix4fv(opengl.vp_location, 1, GL_FALSE, &camera.vp[0][0]);
		glUniformMatrix4fv(opengl.model_location, 1, GL_FALSE, &model[0][0]);

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, water.texture_id);
        opengl._water_plane.draw();

		glfw.post_render();
	}

	return 0;
}
