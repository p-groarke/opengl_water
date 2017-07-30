#pragma once
#include "globals.h"

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>

struct Component {
	virtual void init() {}
	virtual void update(float) {}
	virtual void render(float) {}
	virtual void destroy() {}

	virtual ~Component() = 0;
};
Component::~Component() {}
