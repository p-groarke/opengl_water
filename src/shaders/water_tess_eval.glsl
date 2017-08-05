#version 410 core
layout(quads, equal_spacing, ccw) in;
in vec2 teUv[];
out vec2 fUv;
out vec4 fWorld_pos;
uniform mat4 VP;
uniform mat4 M;
uniform float time;
const float M_PI = 3.1415926535;

float water_displacement(vec2 pos) {
	const float freq = 0.8;
	float d = length(pos + vec2(50, 500));
	return (0.5 + 0.4 * sin(-M_PI * d * freq + time));
}

void main() {
	vec4 p1 = mix(gl_in[0].gl_Position,
			gl_in[1].gl_Position,
			gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position,
			gl_in[3].gl_Position,
			gl_TessCoord.x);
	vec4 local_pos = mix(p2, p1, gl_TessCoord.y);
	fWorld_pos = M * local_pos;
	fWorld_pos.y += water_displacement(fWorld_pos.xz);
	gl_Position = VP * fWorld_pos;

	vec2 uv1 = mix(teUv[0], teUv[1], gl_TessCoord.x);
	vec2 uv2 = mix(teUv[2], teUv[3], gl_TessCoord.x);
	fUv = mix(uv2, uv1, gl_TessCoord.y);
}
