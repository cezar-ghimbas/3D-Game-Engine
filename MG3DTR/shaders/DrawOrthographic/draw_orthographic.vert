#version 330
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
	gl_Position = P * V * M * vec4(inPos, 0, 1);
}