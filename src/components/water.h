#pragma once
#include "engine/component.h"

struct Transform;
struct Renderer;

struct Water : public Component {
	void init() override;
	void update(float dt) override;
	void render(float dt) override;
	void destroy() override;

private:
	Transform* _transform;
	Renderer* _renderer;

	GLuint vertex_array, vertex_buffer, uv_buffer;
	GLint vp_location, vpos_location, model_location, vuv_location
			, time_loc;
};
