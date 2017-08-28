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

	void gen_positions(std::vector<glm::vec3>& positions, int count) {
		int offset_y = -(count / 2);
		for (int y = 0; y < count; ++y) {
			offset_y += 1;
			int offset_x = -(count / 2);
			for (int x = 0; x < count; ++x) {
				offset_x += 1;
				positions.emplace_back(glm::vec3{offset_x, offset_y, 0.f});
			}
		}
	}
}

void Water::init() {
	int num_quads = 2;

	_instance_transforms.reserve(num_quads * num_quads);
	gen_positions(_instance_transforms, num_quads);

	_transform = entity.add_component<Transform>();
	_transform->rotation = glm::rotate(_transform->rotation,
			glm::radians(90.f), { 1.f, 0.f, 0.f });
//	_transform->scale = { 5.f, 5.f, 5.f };
	_transform->scale = { 2.f, 2.f, 2.f };
//	_transform->scale = { 3.f, 3.f, 3.f };

	_renderer = add_component<Renderer>();
	_renderer->load_shader(GL_VERTEX_SHADER, "water_vertex.glsl");
	_renderer->load_shader(GL_TESS_CONTROL_SHADER, "water_tess_control.glsl");
	_renderer->load_shader(GL_TESS_EVALUATION_SHADER, "water_tess_eval.glsl");
	_renderer->load_shader(GL_FRAGMENT_SHADER, "water_fragment.glsl");
	_renderer->create();

	auto& program = _renderer->program;
	vp_loc = glGetUniformLocation(program, "VP");
	model_loc = glGetUniformLocation(program, "M");
	time_loc = glGetUniformLocation(program, "time");
	camera_pos_loc = glGetUniformLocation(program, "camera_pos");
	light_pos_loc = glGetUniformLocation(program, "light_pos");

	vpos_loc = glGetAttribLocation(program, "vPos");
//		vcol_loc = glGetAttribLocation(program, "vCol");
	vuv_loc = glGetAttribLocation(program, "vUv");
	transform_loc = glGetAttribLocation(program, "vTransform");

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	/* Vertex quad */
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _quads.size(), _quads.data()
//			, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vpos_loc);
	glVertexAttribPointer(vpos_loc, 3, GL_FLOAT, GL_FALSE,
			0, (void*)0);

	/* Transforms */
	glGenBuffers(1, &transform_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, transform_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _instance_transforms.size(),
			_instance_transforms.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(transform_loc);
	glVertexAttribPointer(transform_loc, 3, GL_FLOAT, GL_FALSE,
			0, (void*)0);
	glVertexAttribDivisor(transform_loc, 1);


//		glEnableVertexAttribArray(vcol_loc);
//		glVertexAttribPointer(vcol_loc, 4, GL_FLOAT, GL_FALSE,
//				0, (void*) (sizeof(float) * 3));

	/* UVs */
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vuv_loc);
	glVertexAttribPointer(vuv_loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

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

	glUniformMatrix4fv(vp_loc, 1, GL_FALSE,
			&Camera::main->get_view_projection()[0][0]);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE,
			&_transform->get_model()[0][0]);
	glUniform1f(time_loc, glfwGetTime());
	// Transform* _cam_trans = Camera::main->entity->get_component<Transform>();
	// glUniform3fv(camera_pos_loc, 1, &_cam_trans->position[0]);

	glBindVertexArray(vertex_array);
//	glDrawArrays(GL_PATCHES, 0, _quads.size() / 3);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, _instance_transforms.size());
	glBindVertexArray(0);
}

void Water::destroy() {
	glDeleteVertexArrays(1, &vertex_array);
}
