#pragma once
#include "engine/component.h"
#include "components/camera.h"
#include "components/transform.h"

struct RotationCamera : public Component<RotationCamera> {
	void init() {
		_camera = add_component<Camera>();
		_transform = get_component<Transform>();
		_transform->position = { 3.f, 2.f, 2.f };
	}

	void update(float dt) {
		glm::quat quat = glm::angleAxis(glm::radians(rotation_speed * dt)
				, glm::vec3{0.f, 1.f, 0.f});
		_transform->position = quat * _transform->position;
		_transform->look_at({0.f, 0.f, 0.f});
	}

	float rotation_speed = 2.f;

private:
	Component<Camera> _camera;
	Component<Transform> _transform;
};
