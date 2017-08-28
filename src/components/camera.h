#pragma once
#include "engine/component.h"
#include "components/transform.h"

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera : public Component<Camera> {
	void init();
	void destroy();

	glm::mat4 get_view();
	glm::mat4 get_projection();
	glm::mat4 get_view_projection();

	float fov = 80.f;
	float near_plane = 0.1f;
	float far_plane = 1000.f;

	static Component<Camera> main;

private:
	Component<Transform> _transform;

	static bool main_camera_set;
};
