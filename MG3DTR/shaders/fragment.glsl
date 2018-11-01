#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

uniform int use_light;

uniform vec3 color;

in vec3 world_pos;
in vec3 world_normal;

void main(){
	
	float light = 0;

	float dif, spec, norm_dot_L, norm_dot_H, recv_light;
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	norm_dot_L = dot(world_normal, L);
	norm_dot_H = dot(world_normal, H);

	recv_light = norm_dot_L > 0 ? norm_dot_L : 0;
	dif = material_kd * recv_light;
	spec = material_ks * recv_light * pow((norm_dot_H > 0 ? norm_dot_H : 0), material_shininess);

	light = 0.2f + dif + spec;

	if (use_light == 1) {
		out_color = vec4(color * light, 1);
	} else {
		out_color = vec4(color, 1);
	}
}
