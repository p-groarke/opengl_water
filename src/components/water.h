#pragma once
#include "engine/component.h"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <vector>

struct Transform;
struct Renderer;

struct Water : public Component<Water> {
	Water(Entity e);

	void init();
	void update(float dt);
	void render(float dt);
	void destroy();

private:
	Component<Transform> _transform;
	Component<Renderer> _renderer;

	GLuint vertex_array, vertex_buffer, uv_buffer, transform_buffer;
	GLint vp_loc, vpos_loc, model_loc, vuv_loc, time_loc, transform_loc,
		  camera_pos_loc, light_pos_loc;

	std::vector<glm::vec3> _instance_transforms;
};
