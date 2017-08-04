#pragma once
#include "engine/component.h"
#include "components/camera.h"
#include "components/transform.h"

struct RotationCamera : public Component {
	void init() override {
		_camera = entity->add_component<Camera>();
		_transform = entity->get_component<Transform>();
		_transform->position = { 3.f, 2.f, 2.f };
	}

	void update(float dt) override {
		glm::vec3 r_axis{0.f, 1.f, 0.f};
		glm::quat quat = glm::angleAxis(glm::radians(rotation_speed * dt)
				, r_axis);
		_transform->position = quat * _transform->position;
	}

	float rotation_speed = 2.f;

private:
	Camera* _camera;
	Transform* _transform;
};
