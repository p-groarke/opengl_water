#include "components/camera.h"

#include "engine/entity.h"
#include "engine/window.h"
#include "components/transform.h"

Camera::Camera()
{
	if (Camera::main == nullptr)
		Camera::main = this;
}

void Camera::init(ts::Entity entity)
{
	_transform = entity.add_component<Transform>();
}

glm::mat4 Camera::get_view()
{
	return glm::lookAt(_transform->position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::get_projection()
{
	return glm::perspective(glm::radians(fov), Window::main->window_ratio, near_plane, far_plane);
}

glm::mat4 Camera::get_view_projection()
{
	return get_projection() * get_view();
}

Camera* Camera::main = nullptr;
