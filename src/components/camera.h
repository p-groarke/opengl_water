#pragma once
#include "engine/component.h"

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform;

struct Camera {
	Camera();
	void init(ts::Entity entity);
	glm::mat4 get_view();
	glm::mat4 get_projection();
	glm::mat4 get_view_projection();

	float fov = 80.f;
	float near_plane = 0.1f;
	float far_plane = 100.f;

	static Camera* main;

private:
	Transform* _transform = nullptr;
};
