#version 410 core
layout(quads, equal_spacing, ccw) in;
in vec2 teUv[];
out vec2 fUv;
out vec4 fWorld_pos;

uniform mat4 VP;
uniform mat4 M;
uniform float time;

const float M_PI = 3.1415926535;
const float steepness = 0.6; // Q : 0 to 1

struct wave {
	float amplitude;	// A
	vec2 direction;		// D
	float wavelength;	// L
	float speed;		// Phi
};
// w = frequency = 2/L
// Qi = actual steepness = Q / w * A

const wave waves[2] = wave[](
	wave(1, vec2(0, 1), 2, 1)
	, wave(0.1, vec2(-1, -2), 0.5, 1)
//	, wave(0.3, vec2(1, 3), 0.2, 0.5)
);

float phase(float speed, float wavelength, float time) {
	return speed * (2 / wavelength) * time;
}

float w(float wavelength) {
	return 2 / wavelength;
}

float Q(float w, float A) {
	return steepness / (w * A);
}

vec3 gerstner_wave(vec2 pos, float time) {
	float x = pos.x;
	for (int i = 0; i < waves.length(); ++i) {
		vec2 dir = normalize(waves[i].direction);
		float w = w(waves[i].wavelength);
		float Qi = Q(w, waves[i].amplitude);
		float phi_t = phase(waves[i].speed, waves[i].wavelength, time);
		x += Qi * waves[i].amplitude * dir.x
				* cos(w * dot(dir, pos) + phi_t);
	}

	float z = pos.y;
	for (int i = 0; i < waves.length(); ++i) {
		vec2 dir = normalize(waves[i].direction);
		float w = w(waves[i].wavelength);
		float Qi = Q(w, waves[i].amplitude);
		float phi_t = phase(waves[i].speed, waves[i].wavelength, time);
		z += Qi * waves[i].amplitude * dir.y
				* cos(w * dot(dir, pos) + phi_t);
	}

	float y = 0;
	for (int i = 0; i < waves.length(); ++i) {
		vec2 dir = normalize(waves[i].direction);
		float w = w(waves[i].wavelength);
		float Qi = Q(w, waves[i].amplitude);
		float phi_t = phase(waves[i].speed, waves[i].wavelength, time);
		y += waves[i].amplitude * sin(w * dot(dir, pos) + phi_t);
	}

	return vec3(x, y, z);
}

void main() {
	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 local_pos = mix(p2, p1, gl_TessCoord.y);
	fWorld_pos = M * local_pos;

	fWorld_pos.xyz = gerstner_wave(fWorld_pos.xz, time);
	gl_Position = VP * fWorld_pos;

	vec2 uv1 = mix(teUv[0], teUv[1], gl_TessCoord.x);
	vec2 uv2 = mix(teUv[2], teUv[3], gl_TessCoord.x);
	fUv = mix(uv2, uv1, gl_TessCoord.y);
}
