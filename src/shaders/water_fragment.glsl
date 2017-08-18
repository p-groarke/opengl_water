#version 410 core
layout (location = 0) out vec4 color;
in vec2 fUv;
in vec4 fWorld_pos;
in vec3 fNormal;
in float fMax_y;
in float fMin_y;

uniform vec3 camera_pos;
uniform vec3 light_pos;

void main() {
	// vec4 dark_base_color = vec4(0.08, 0.16, 0.16, 1.0);
	// vec4 light_base_color = dark_base_color + 0.3;// vec4(0.73, 0.77, 0.80, 1.0);
	vec4 dark_base_color = vec4(49.0 / 255.0, 86.0 / 255.0, 120.0 / 255.0, 1.0);
	vec4 light_base_color = vec4(153.0 / 255.0, 224.0 / 255.0, 255.0 / 255.0, 1.0);

	float percentage = (fWorld_pos.y - fMin_y) / (fMax_y - fMin_y);
//	percentage = clamp(percentage / 3, 0, 1);
	vec4 c = mix(dark_base_color, light_base_color, percentage);
	color = c;
}
