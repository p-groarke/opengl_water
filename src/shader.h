#pragma once

#define GLSL(...) "#version 410 core\n"#__VA_ARGS__//\0

static const char* vertex_shader_source = GLSL(
	layout (location = 0)	in vec3 vPos;
	layout (location = 1)	in vec2 vUv;
	out vec2 tcUv;

	void main() {
		tcUv = vUv;
		gl_Position = vec4(vPos, 1.0);
	}\0
);

static const char* tess_control_shader_source = GLSL(
	layout (vertices = 4) out;
	in vec2 tcUv[];
	out vec2 teUv[];

	void main() {
		if (gl_InvocationID == 0) {
			gl_TessLevelInner[0] = 50.0;
			gl_TessLevelInner[1] = 50.0;
			gl_TessLevelOuter[0] = 50.0;
			gl_TessLevelOuter[1] = 50.0;
			gl_TessLevelOuter[2] = 50.0;
			gl_TessLevelOuter[3] = 50.0;
		}

		gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
		teUv[gl_InvocationID] = tcUv[gl_InvocationID];
	}\0
);

static const char* tess_eval_shader_source = GLSL(
	layout(quads, equal_spacing, ccw) in;
	in vec2 teUv[];
	out vec2 fUv;
	uniform mat4 VP;
	uniform mat4 M;
	uniform sampler2D water_disp;

	void main() {
		vec4 p1 = mix(gl_in[0].gl_Position,
				gl_in[1].gl_Position,
				gl_TessCoord.x);
		vec4 p2 = mix(gl_in[2].gl_Position,
				gl_in[3].gl_Position,
				gl_TessCoord.x);

		vec4 local_pos = mix(p1, p2, gl_TessCoord.y);
		local_pos.z += texture(water_disp, gl_TessCoord.xy).r * 0.1;

		gl_Position = VP * M * local_pos;
		fUv = gl_TessCoord.xy;
	}\0
);

static const char* fragment_shader_source = GLSL(
	layout (location = 0) out vec4 color;
	in vec2 fUv;
	uniform sampler2D water_disp;

	void main() {
		color = vec4(0.02, 0.1, 1.0 - texture(water_disp, fUv).r * 0.5, 1.0);
	}\0
);
