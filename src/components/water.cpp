#include "components/water.h"
#include "engine/entity.h"
#include "engine/window.h"
#include "components/transform.h"
#include "components/renderer.h"
#include "components/camera.h"

namespace {
static const GLfloat quad[] = {
	-1.f, -1.f, 0.0f, // Bottom-left
	1.f, -1.f, 0.0f, // Bottom-right
	-1.f, 1.f, 0.0f, // Top-left
	1.f, 1.f, 0.0f // Top-right
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
} // namespace

void Water::init(ts::Entity entity)
{
	_transform = entity.add_component<Transform>();
	_transform->rotation.x = 90.f;
	_transform->scale = { 5.f, 5.f, 5.f };

	_renderer = entity.add_component<Renderer>();
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

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

void Water::update(ts::Entity, float)
{
}

void Water::render(ts::Entity, float)
{
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

	glUniformMatrix4fv(vp_location, 1, GL_FALSE, &Camera::main->get_view_projection()[0][0]);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, &_transform->get_model()[0][0]);
	glUniform1f(time_loc, glfwGetTime());

	glBindVertexArray(vertex_array);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}

void Water::destroy(ts::Entity)
{
	glDeleteVertexArrays(1, &vertex_array);
}
