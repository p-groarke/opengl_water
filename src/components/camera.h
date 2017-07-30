#pragma once
#include "engine/component.h"

struct Camera : public Component {
	void init() {

//		vp_loc = glGetUniformLocation(program, "VP");
	}

	void update(float dt) {
		glm::vec3 r_axis{0.f, 1.f, 0.f};
		glm::quat quat = glm::angleAxis(glm::radians(rotation_speed * dt)
				, r_axis);
		position = quat * position;

		view = glm::lookAt(position, glm::vec3(0.f, 0.f, 0.f)
				, glm::vec3(0.f, 1.f, 0.f));
		projection = glm::perspective(glm::radians(fov)
				, Glfw::window_ratio, 0.1f, 100.0f);
		vp = projection * view;
	}

	const float fov = 80.f;
	const float rotation_speed = 2.f;
	glm::vec3 position{3.f, 2.f, 2.f};
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 vp;

	GLint vp_loc;
};

