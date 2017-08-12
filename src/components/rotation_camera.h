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
		glm::quat quat = glm::angleAxis(glm::radians(rotation_speed * dt)
				, glm::vec3{0.f, 1.f, 0.f});
		_transform->position = quat * _transform->position;
		_transform->look_at({0.f, 0.f, 0.f});
	}

	float rotation_speed = 2.f;

private:
	Camera* _camera;
	Transform* _transform;
};
