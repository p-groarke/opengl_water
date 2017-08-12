#pragma once
#include "engine/component.h"
#include "components/transform.h"
#include "components/camera.h"

struct EditorCamera : public Component {
	void init() override {
		_camera = entity->add_component<Camera>();
		_transform = entity->get_component<Transform>();
		_transform->position = { 3.f, 2.f, 2.f };
	}

	void update() override {

	}

private:
	Transform* _transform;
	Camera* _camera;
};
