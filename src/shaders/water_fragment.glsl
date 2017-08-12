#version 410 core
layout (location = 0) out vec4 color;
in vec2 fUv;
in vec4 fWorld_pos;
uniform sampler2D water_disp;

void main() {
	vec4 dark_base_color = vec4(0.08, 0.16, 0.16, 1.0);
	vec4 light_base_color = vec4(0.73, 0.77, 0.80, 1.0);
//	float disp = texture(water_disp, fUv).r;
//	vec4 c = mix(dark_base_color, light_base_color, pow(disp, 2.0));

	float percentage = clamp(fWorld_pos.y, 0, 1);
	vec4 c = mix(dark_base_color, light_base_color, pow(percentage, 2.0));
	color = c;
}
