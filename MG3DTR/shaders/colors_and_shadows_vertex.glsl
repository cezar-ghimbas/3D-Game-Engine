#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec3 world_pos;
out vec3 world_normal;

uniform mat4 lightSpaceMatrix;
out vec4 light_space;


void main(){

	world_pos = (model_matrix * vec4(in_position, 1)).xyz;
	//world_normal = (normalize(model_matrix * vec4(in_normal, 0))).xyz;
	world_normal = transpose(inverse(mat3(model_matrix))) * in_normal;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_position,1);
	//light_space = lightSpaceMatrix * model_matrix * vec4(in_position,1);
	light_space = lightSpaceMatrix * vec4(world_pos, 1);
}