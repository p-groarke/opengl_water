#pragma once

#define GLSL(...) "#version 410 core\n"#__VA_ARGS__

static const char* vertex_shader_source = GLSL(
	layout (location = 0)	in vec3 vPos;
//	layout (location = 1)	in vec4 vCol;
	layout (location = 2)	in vec2 water_uv;
	uniform mat4 VP;
	uniform mat4 M;

	uniform sampler2D water_displacement;

//	out vec4 fCol;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	out vec2 uv;

	void main() {
		vec3 pos = vPos;
		pos.z = texture(water_displacement, water_uv).r * 0.1;
		gl_Position = VP * M * vec4(pos, 1.0);
		uv = water_uv;
//		gl_Position = vec4(vPos, 1.0);
//		fCol = vCol;
	}\0
);

static const char* fragment_shader_source = GLSL(
//	in vec4 fCol;
	layout (location = 0) out vec4 color;

	uniform sampler2D water_displacement;
	in vec2 uv;

	void main() {

//		color = ;
		color = vec4(0.02, 0.1, 0.5 + 1.0 - texture(water_displacement, uv).r * 0.5, 0.9f);
	}\0
);
