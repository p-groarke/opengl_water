#pragma once

#define GLSL(...) "#version 410 core\n"#__VA_ARGS__"\0"

static const char* vertex_shader_source = GLSL(
	layout (location = 0)	in vec3 vPos;
	layout (location = 1)	in vec2 vUv;
	out vec2 tcUv;

	void main() {
		tcUv = vUv;
		gl_Position = vec4(vPos, 1.0);
	}
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
	}
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
		vec4 world_pos = M * mix(p2, p1, gl_TessCoord.y);

		vec2 uv1 = mix(teUv[0], teUv[1], gl_TessCoord.x);
		vec2 uv2 = mix(teUv[2], teUv[3], gl_TessCoord.x);
		fUv = mix(uv2, uv1, gl_TessCoord.y);

		world_pos.y += texture(water_disp, fUv).r * 0.5;
		gl_Position = VP * world_pos;
	}
);

static const char* fragment_shader_source = GLSL(
	layout (location = 0) out vec4 color;
	in vec2 fUv;
	uniform sampler2D water_disp;

	void main() {
		vec4 dark_base_color = vec4(0.08, 0.16, 0.16, 1.0);
		vec4 light_base_color = vec4(0.73, 0.77, 0.80, 1.0);
		float disp = texture(water_disp, fUv).r;
		vec4 c = mix(dark_base_color, light_base_color, pow(disp, 2.0));
		color = c;
	}
);
