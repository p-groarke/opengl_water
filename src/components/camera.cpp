#include "components/camera.h"

#include "engine/entity.h"
#include "engine/window.h"
#include "components/transform.h"

#include <glm/gtc/matrix_inverse.hpp>

Camera::Camera(Entity e)
	: Component<Camera>(e)
	, _transform(e)
{
	if (!Camera::main_camera_set) {
		Camera::main = *this;
		Camera::main_camera_set = true;
	}
}

void Camera::init() {
	_transform = add_component<Transform>();
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

Component<Camera> Camera::main{ Entity::dummy };
bool Camera::main_camera_set = false;
