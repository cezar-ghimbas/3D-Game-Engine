#version 330
//layout(location = 0) in vec3 inPos;

uniform mat4 proj_mat, view_mat;
uniform vec3 point;

void main() {
	gl_PointSize = 10;
	gl_Position = proj_mat * view_mat * vec4(point, 1);
}