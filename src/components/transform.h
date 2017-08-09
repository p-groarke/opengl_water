#pragma once
#include "engine/component.h"

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
	/* Or this could be done in update... Probly not. */
	glm::mat4 get_model()
	{
		glm::mat4 ret{ 1.f };
		ret = glm::scale(ret, scale);

		glm::quat _quat(glm::radians(rotation));
		ret *= glm::mat4_cast(_quat);

		ret = glm::translate(ret, position);
		return ret;
	}

	static constexpr bool is_data()
	{
		return true;
	}

	glm::vec3 position{ 0.f, 0.f, 0.f };
	glm::vec3 rotation{ 0.f, 0.f, 0.f };
	glm::vec3 scale{ 1.f, 1.f, 1.f };
};
