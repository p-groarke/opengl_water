#version 410 core
layout(quads, equal_spacing, ccw) in;
in vec2 teUv[];
out vec2 fUv;
out vec4 fWorld_pos;
out vec3 fNormal;
out float fMax_y;
out float fMin_y;

uniform mat4 VP;
uniform mat4 M;
uniform float time;

const float M_PI = 3.1415926535;

// w = frequency = 2/L
float w(float L) {
	return 2 / L;
}

// Qi = actual steepness = Q / w * A (* i)?
float Q(float steepness, float w, float A, float num_waves) {
	return steepness / (w * A * num_waves);
}

// Phi = speed * frequency(w)
float Phi(float speed, float w) {
	return speed * w;
}

struct WaveConstants {
	float steepness;	// Q : 0 to 1
	float A;			// Amplitude
	vec2 D;				// Direction : normalized
	float L;			// wavelength
	float speed;		// Phi
};

struct Wave {
	WaveConstants constants;
	float w;			// w = frequency = 2 / L
	float Q;			// Qi = actual steepness = Q / w * A (* i)?
	float phi;			// Phi = speed * w
};

const WaveConstants wave_constants[] = WaveConstants[](
	  WaveConstants(0.9, 0.1, normalize(vec2(1, 0)), 1, 1)
	, WaveConstants(0.9, 0.2, normalize(vec2(1, 0)), 2, 2)
	, WaveConstants(0.9, 0.4, normalize(vec2(1, 0)), 4, 4)
	, WaveConstants(0.9, 0.8, normalize(vec2(1, 0)), 8, 8)
	, WaveConstants(0.6, 0.05, normalize(vec2(1, 1)), 1, 1)
	, WaveConstants(0.6, 0.1, normalize(vec2(1, 1)), 2, 2)
	, WaveConstants(0.6, 0.2, normalize(vec2(1, 1)), 4, 4)
	, WaveConstants(0.6, 0.4, normalize(vec2(1, 1)), 8, 8)
	//WaveConstants(0.8, 0.3, normalize(vec2(1, 0)), 3, 7)
	//, WaveConstants(0.5, 0.1, normalize(vec2(1, 0.2)), 1, 3)
	//, WaveConstants(0.8, 0.2, normalize(vec2(1, -0.3)), 5, 5)
	//, WaveConstants(0.01, 1.5, normalize(vec2(1, -3)), 50, 25)
	//, WaveConstants(0.6, 0.2, normalize(vec2(0, 5)), 10, 0.8)
	//, WaveConstants(1, 2, normalize(vec2(1, 0)), 40, 20)
	//, WaveConstants(0.2, 0.3, normalize(vec2(1, 0.5)), 15, 15)
	//, WaveConstants(0.4, 0.2, normalize(vec2(-1, 0.01)), 2.5, 0.1)
);
const uint num_waves = wave_constants.length();

// As long as it works and is fast...
Wave waves[] = Wave[](
	Wave(
		wave_constants[0]
		, w(wave_constants[0].L)
		, Q(wave_constants[0].steepness, w(wave_constants[0].L),
				wave_constants[0].A, num_waves)
		, Phi(wave_constants[0].speed, w(wave_constants[0].L))
	)
	, Wave(
		wave_constants[1]
		, w(wave_constants[1].L)
		, Q(wave_constants[1].steepness, w(wave_constants[1].L),
				wave_constants[1].A, num_waves)
		, Phi(wave_constants[1].speed, w(wave_constants[1].L))
	)
	, Wave(
		wave_constants[2]
		, w(wave_constants[2].L)
		, Q(wave_constants[2].steepness, w(wave_constants[2].L),
				wave_constants[2].A, num_waves)
		, Phi(wave_constants[2].speed, w(wave_constants[2].L))
	)
	, Wave(
		wave_constants[3]
		, w(wave_constants[3].L)
		, Q(wave_constants[3].steepness, w(wave_constants[3].L),
				wave_constants[3].A, num_waves)
		, Phi(wave_constants[3].speed, w(wave_constants[3].L))
	)
	, Wave(
		wave_constants[4]
		, w(wave_constants[4].L)
		, Q(wave_constants[4].steepness, w(wave_constants[4].L),
				wave_constants[4].A, num_waves)
		, Phi(wave_constants[4].speed, w(wave_constants[4].L))
	)
	, Wave(
		wave_constants[5]
		, w(wave_constants[5].L)
		, Q(wave_constants[5].steepness, w(wave_constants[5].L),
				wave_constants[5].A, num_waves)
		, Phi(wave_constants[5].speed, w(wave_constants[5].L))
	)
	, Wave(
		wave_constants[6]
		, w(wave_constants[6].L)
		, Q(wave_constants[6].steepness, w(wave_constants[6].L),
				wave_constants[6].A, num_waves)
		, Phi(wave_constants[6].speed, w(wave_constants[6].L))
	)
	, Wave(
		wave_constants[7]
		, w(wave_constants[7].L)
		, Q(wave_constants[7].steepness, w(wave_constants[7].L),
				wave_constants[7].A, num_waves)
		, Phi(wave_constants[7].speed, w(wave_constants[7].L))
	)
);

vec3 gerstner_wave(vec2 pos,
		out float cos_terms[num_waves],
		out float sin_terms[num_waves])
{
	float x = pos.x;
	for (int i = 0; i < waves.length(); ++i) {
		cos_terms[i] = cos(waves[i].w * dot(waves[i].constants.D, pos)
				+ waves[i].phi * time);
		x += waves[i].Q * waves[i].constants.A * waves[i].constants.D.x
				* cos_terms[i];
	}

	float z = pos.y;
	for (int i = 0; i < waves.length(); ++i) {
		z += waves[i].Q * waves[i].constants.A * waves[i].constants.D.y
				* cos_terms[i];
	}

	float y = 0;
	for (int i = 0; i < waves.length(); ++i) {
		sin_terms[i] = sin(waves[i].w * dot(waves[i].constants.D, pos)
				+ waves[i].phi * time);
		y += waves[i].constants.A
				* sin_terms[i];
	}

	return vec3(x, y, z);
}

vec3 grestner_normal(float cos_terms[num_waves], float sin_terms[num_waves]) {
	float x = 0;
	for (int i = 0; i < waves.length(); ++i) {
		x += waves[i].constants.D.x * waves[i].w * waves[i].constants.A
				* cos_terms[i];
	}

	float z = 0;
	for (int i = 0; i < waves.length(); ++i) {
		z += waves[i].constants.D.y * waves[i].w * waves[i].constants.A
				* cos_terms[i];
	}

	float y = 0;
	for (int i = 0; i < waves.length(); ++i) {
		y += waves[i].Q * waves[i].w * waves[i].constants.A * sin_terms[i];
	}

	return vec3(-x, 1 - y, -z);
}

float grestner_max_y() {
	float y = 0;
	for (int i = 0; i < waves.length(); ++i) {
		y += waves[i].constants.A;
	}
	return y;
}

float grestner_min_y() {
	float y = 0;
	for (int i = 0; i < waves.length(); ++i) {
		y -= waves[i].constants.A;
	}
	return y;
}

void main() {
	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 local_pos = mix(p2, p1, gl_TessCoord.y);
	fWorld_pos = M * local_pos;

	float cos_terms[num_waves];
	float sin_terms[num_waves];
	fWorld_pos.xyz = gerstner_wave(fWorld_pos.xz, cos_terms, sin_terms);
	gl_Position = VP * fWorld_pos;

	vec2 uv1 = mix(teUv[0], teUv[1], gl_TessCoord.x);
	vec2 uv2 = mix(teUv[2], teUv[3], gl_TessCoord.x);
	fUv = mix(uv2, uv1, gl_TessCoord.y);

	fNormal = grestner_normal(cos_terms, sin_terms);
	fMax_y = grestner_max_y();
	fMin_y = grestner_min_y();
}
