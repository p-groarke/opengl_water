#pragma once
#include "engine/component.h"

#include <glm/vec3.hpp>
#include <vector>

struct Transform;
struct Renderer;

struct Water : public Component {
	void init() override;
	void update(float dt) override;
	void render(float dt) override;
	void destroy() override;

private:
	Transform* _transform;
	Renderer* _renderer;

	GLuint vertex_array, vertex_buffer, uv_buffer, transform_buffer;
	GLint vp_loc, vpos_loc, model_loc, vuv_loc, time_loc, transform_loc;

	std::vector<glm::vec3> _instance_transforms;
};
