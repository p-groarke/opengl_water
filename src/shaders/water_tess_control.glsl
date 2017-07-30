#version 410 core\n
layout (vertices = 4) out;
in vec2 tcUv[];
out vec2 teUv[];
//uniform sampler2D water_disp;

void main() {
	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = 50.0;
		gl_TessLevelInner[1] = 50.0;
		gl_TessLevelOuter[0] = 50.0;
		gl_TessLevelOuter[1] = 50.0;
		gl_TessLevelOuter[2] = 50.0;
		gl_TessLevelOuter[3] = 50.0;
	}

//	vec4 pos = gl_in[gl_InvocationID].gl_Position;
//	pos.z += texture(water_disp, tcUv[gl_InvocationID]).r * 0.5;
//	gl_out[gl_InvocationID].gl_Position = pos;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	teUv[gl_InvocationID] = tcUv[gl_InvocationID];
}