#pragma once
#include "engine/component.h"
#include "components/transform.h"
#include "components/camera.h"

#include <array>
#include <utility>

struct EditorCamera : public Component<EditorCamera> {
	void init();
	void destroy();
	void update(float dt);

private:
	void on_mouse_move(double x, double y);
	void on_mouse_scroll(double x, double y);
	void on_mouse_click(int button, int action, int mods);

public:
	float mouse_speed = 15.f;
	float move_speed = 15.f;

private:
	Component<Transform> _transform;
	Component<Camera> _camera;

	bool _left_click = false;
	bool _right_click = false;
	bool _middle_click = false;
	float _last_dt = 0.f;
	glm::dvec2 _last_mouse_pos = { 0.f, 0.f };

	/* TODO : Event cleanup */
//	std::array<std::function<void()>, 3> _cleanup_callbacks;
};
