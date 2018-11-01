#version 330
layout (location = 0) in vec3 inPos;

uniform mat4 model_matrix;

void main() {
	gl_Position = model_matrix * vec4(inPos, 1);
}