#include "components/camera.h"

#include "engine/entity.h"
#include "engine/window.h"
#include "components/transform.h"

#include <glm/gtc/matrix_inverse.hpp>

Camera::Camera() {
	if (Camera::main == nullptr)
		Camera::main = this;
}

void Camera::init() {
	_transform = entity->add_component<Transform>();
}

glm::mat4 Camera::get_view() {
	return glm::affineInverse(_transform->get_model());
}

glm::mat4 Camera::get_projection() {
	return glm::perspective(glm::radians(fov), Window::main->window_ratio
			, near_plane, far_plane);
}

glm::mat4 Camera::get_view_projection() {
	return get_projection() * get_view();
}

Camera* Camera::main = nullptr;
