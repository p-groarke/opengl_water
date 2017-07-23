#pragma once

#define GLSL(...) "#version 410 core\n"#__VA_ARGS__

static const char* vertex_shader_source = GLSL(
	layout (location = 0)	in vec3 vPos;
	layout (location = 1)	in vec4 vCol;
	uniform mat4 VP;
	out vec4 fCol;

	out gl_PerVertex {
		vec4 gl_Position;
	};

	void main() {
		gl_Position = VP * vec4(vPos, 1.0);
//		gl_Position = vec4(vPos, 1.0);
		fCol = vCol;
	}\0
);

static const char* fragment_shader_source = GLSL(
	in vec4 fCol;
	layout (location = 0) out vec4 color;

	void main() {
		color = vec4(0.86, 0.62, 0.86, 0.9f);
	}\0
);
