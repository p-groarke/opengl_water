#include "components/water.h"
#include "engine/entity.h"
#include "engine/window.h"
#include "components/transform.h"
#include "components/renderer.h"
#include "components/camera.h"

namespace {
	static const GLfloat quad[] = {
		-0.5f, -0.5f, 0.0f, // Bottom-left
		0.5f, -0.5f, 0.0f, // Bottom-right
		-0.5f, 0.5f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f // Top-right
	//	-0.5f, 0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.5f, 0.5f, 0.0f
	};

	static const float tex_coords[] = {
		0.0, 0.0, // Bottom-left.
		1.0, 0.0, // Bottom-right.
		0.0, 1.0, // Top-left.
		1.0, 1.0 // Top-right.
	};

	void init_big_quad(std::vector<GLfloat>& quads, int count) {
		int offset_y = -(count / 2);
		for (int y = 0; y < count; ++y) {
			offset_y += 1;
			int offset_x = -(count / 2);
			for (int x = 0; x < count; ++x) {
				offset_x += 1;
				quads.push_back(quad[0] + offset_x);
				quads.push_back(quad[1] + offset_y);
				quads.push_back(quad[2]);
				quads.push_back(quad[3] + offset_x);
				quads.push_back(quad[4] + offset_y);
				quads.push_back(quad[5]);
				quads.push_back(quad[6] + offset_x);
				quads.push_back(quad[7] + offset_y);
				quads.push_back(quad[8]);
				quads.push_back(quad[9] + offset_x);
				quads.push_back(quad[10] + offset_y);
				quads.push_back(quad[11]);
			}
		}
	}
}

void Water::init() {
	int num_quads = 2;
	_quads.reserve(num_quads * num_quads);
	init_big_quad(_quads, num_quads);

	_transform = entity->add_component<Transform>();
	_transform->rotation = glm::rotate(_transform->rotation,
			glm::radians(90.f), { 1.f, 0.f, 0.f });
//	_transform->scale = { 5.f, 5.f, 5.f };
//	_transform->scale = { 2.f, 2.f, 2.f };
	_transform->scale = { 3.f, 3.f, 3.f };

	_renderer = entity->add_component<Renderer>();
	_renderer->set_shader_path("shaders/");
	_renderer->load_shader(GL_VERTEX_SHADER, "water_vertex.glsl");
	_renderer->load_shader(GL_TESS_CONTROL_SHADER, "water_tess_control.glsl");
	_renderer->load_shader(GL_TESS_EVALUATION_SHADER, "water_tess_eval.glsl");
	_renderer->load_shader(GL_FRAGMENT_SHADER, "water_fragment.glsl");
	_renderer->create();

	auto& program = _renderer->program;
	vp_location = glGetUniformLocation(program, "VP");
	model_location = glGetUniformLocation(program, "M");
	time_loc = glGetUniformLocation(program, "time");

	vpos_location = glGetAttribLocation(program, "vPos");
//		vcol_location = glGetAttribLocation(program, "vCol");
	vuv_location = glGetAttribLocation(program, "vUv");

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _quads.size(), _quads.data()
			, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			0, (void*)0);

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
}

void Water::update(float) {

}

void Water::render(float) {
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
//	glEnable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);


	glUseProgram(_renderer->program);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glUniformMatrix4fv(vp_location, 1, GL_FALSE,
			&Camera::main->get_view_projection()[0][0]);
	glUniformMatrix4fv(model_location, 1, GL_FALSE,
			&_transform->get_model()[0][0]);
	glUniform1f(time_loc, glfwGetTime());

	glBindVertexArray(vertex_array);
	glDrawArrays(GL_PATCHES, 0, _quads.size() / 3);
	glBindVertexArray(0);
}

void Water::destroy() {
	glDeleteVertexArrays(1, &vertex_array);
}
