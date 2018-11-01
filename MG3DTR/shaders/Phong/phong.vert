#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
		
uniform mat4 modelMatrix, viewMatrix, projectionMatrix;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoords;

void main(){
	
	worldPos = (modelMatrix * vec4(inPosition, 1)).xyz;
	worldNormal = (normalize(modelMatrix * vec4(inNormal, 0))).xyz;
	texCoords = inTexCoords;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition,1); 
}
