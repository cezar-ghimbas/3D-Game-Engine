#version 330

layout(points) in;
layout(line_strip, max_vertices = 3) out;

uniform mat4 proj_mat, view_mat;
uniform vec3 endPoint;

void main() {
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	gl_Position = proj_mat * view_mat * vec4(endPoint, 1);
	EmitVertex();
	
	EndPrimitive();
}