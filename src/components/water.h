#pragma once
#include "engine/entity.h"

class Transform;
class Renderer;

class Water {
public:
	void init(ts::Entity entity);
	void update(ts::Entity entity, float dt);
	void render(ts::Entity entity, float dt);
	void destroy(ts::Entity entity);

private:
	Transform* _transform;
	Renderer* _renderer;

	GLuint vertex_array, vertex_buffer, uv_buffer;
	GLint vp_location, vpos_location, model_location, vuv_location, time_loc;
};
