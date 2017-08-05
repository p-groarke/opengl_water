#version 410 core
layout (vertices = 4) out;
in vec2 tcUv[];
out vec2 teUv[];

void main() {
	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = 64;
		gl_TessLevelInner[1] = 64;
		gl_TessLevelOuter[0] = 64;
		gl_TessLevelOuter[1] = 64;
		gl_TessLevelOuter[2] = 64;
		gl_TessLevelOuter[3] = 64;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	teUv[gl_InvocationID] = tcUv[gl_InvocationID];
}
