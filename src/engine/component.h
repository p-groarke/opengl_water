#pragma once
#include "engine/globals.h"

struct Entity;

struct Component {
	virtual void init() {}
	virtual void update(float) {}
	virtual void render(float) {}
	virtual void destroy() {}

	Entity* entity = nullptr;
};
