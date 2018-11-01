#version 330
//layout(location = 0) out vec4 outColor;

layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 norm;
layout(location = 2) out vec4 col;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 eyePosition;

uniform vec3 color;

uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform float shininess;

uniform sampler2D objTexture;

in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoords;

void main() {

	vec4 outColor;
	vec3 normal = normalize(worldNormal);

	vec3 ambientColor = lightColor * ambient;
	vec3 lightDirection = normalize(lightPosition - worldPos);
	vec3 viewDirection = normalize(eyePosition - worldPos);
	vec3 halfDirection = normalize(lightDirection + viewDirection);

	vec3 diffuseColor = lightColor * diffuse * max(dot(normal, lightDirection), 0.0);
	vec3 specularColor = lightColor * specular * pow(max(dot(normal, halfDirection), 0.0), shininess);

	outColor = vec4(ambientColor + specularColor + diffuseColor, 1.0);
	outColor += vec4(texture(objTexture, texCoords).xyz, 0.0);

	pos = worldPos;
	norm = normal;
	col.xyz = outColor.xyz;
}

/*
#version 330 core
layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 norm;
layout(location = 2) out vec4 col;
//layout(location = 3) out vec3 ssao;

in vec2 texCoords;
in vec3 worldPos;
in vec3 worldNorm;

uniform vec3 color;

uniform sampler2D screenTexture;

void main()
{
	pos = worldPos;
	norm = normalize(worldNorm);
	col.xyz = color;
	//ssao = worldPos;
}
*/