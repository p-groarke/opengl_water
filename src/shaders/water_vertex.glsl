#version 410 core
layout (location = 0)	in vec3 vPos;
layout (location = 1)	in vec2 vUv;
layout (location = 2)	in vec3 vTransform;
out vec2 tcUv;

void main() {
	tcUv = vUv;
	mat4 transform = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		vTransform.x, vTransform.y, vTransform.x, 1
	);
	gl_Position = transform * vec4(vPos, 1.0);
}
