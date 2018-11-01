#version 330
layout(location = 0) in vec3 inPos;

uniform mat4 proj_mat, view_mat;

void main() {
	//gl_PointSize = 20;
	gl_Position = proj_mat * view_mat * vec4(inPos, 1);
}