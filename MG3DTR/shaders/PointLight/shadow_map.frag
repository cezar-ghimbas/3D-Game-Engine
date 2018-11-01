#version 330 core
in vec4 fragPos;

uniform vec3 light_position;
uniform float farPlane;

void main() {
	float lightDistance = length(fragPos.xyz - light_position);

	lightDistance = lightDistance / farPlane;

	gl_FragDepth = lightDistance;
}