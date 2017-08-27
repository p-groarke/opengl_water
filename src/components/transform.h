#pragma once
#include "engine/component.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

struct Transform : public Component<Transform> {
	void look_at(const glm::vec3& target,
				const glm::vec3& up = { 0.f, 1.f, 0.f })
	{
		glm::mat4 m = glm::lookAt(position, target, up);
		rotation = glm::conjugate(glm::quat_cast(m));
	}

	glm::mat4 get_model() {
		glm::mat4 ret{ 1.f };
		glm::mat4 _scale = glm::scale(ret, scale);
		glm::mat4 _rotate = glm::mat4_cast(rotation);
		glm::mat4 _translate = glm::translate(ret, position);
		return _translate * _rotate * _scale;
	}

	glm::vec3 up() {
		return rotation * glm::vec3{ 0.f, 1.f, 0.f };
	}

	glm::vec3 forward() {
		return rotation * glm::vec3{ 0.f, 0.f, -1.f };
	}

	glm::vec3 right() {
		return rotation * glm::vec3{ 1.f, 0.f, 0.f };
	}

	glm::vec3 position{ 0.f, 0.f, 0.f };
	glm::quat rotation{};
	glm::vec3 scale{ 1.f, 1.f, 1.f };
};
