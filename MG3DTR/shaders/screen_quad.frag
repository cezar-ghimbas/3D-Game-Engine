#version 330 core
in vec2 fragTexCoords;
out vec4 color;

uniform sampler2D posTexture;
uniform sampler2D normTexture;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D ssaoTexture;
uniform sampler2D blurTexture;

uniform float nearPlane, farPlane;
uniform int drawType;

uniform vec3 lightPos;
uniform vec3 eyePos;
uniform int materialShininess;
uniform float materialKd;
uniform float materialKs;

//uniform vec3 sampleOffsets[10];

uniform mat4 P, V;

float LinearCoords(float depth) {
	depth = 2 * depth - 1;
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
}

void main()
{
	if (drawType == 1) {
		//color = vec4(texture(ssaoTexture, fragTexCoords).xyz, 1.0);
		color = vec4(texture(blurTexture, fragTexCoords).xyz, 1.0);
	} else if (drawType == 2) {
		color = vec4(texture(normTexture, fragTexCoords).xyz, 1.0);
	} else if (drawType == 3) {
		float depth = texture(depthTexture, fragTexCoords).x;
		color = vec4(vec3(LinearCoords(depth)) / farPlane, 1.0);
	} else if (drawType == 4) {
		color = vec4(texture(colorTexture, fragTexCoords).xyz, 1.0);
	} else if (drawType == 5) {
		
		vec3 worldPos = texture(posTexture, fragTexCoords).xyz;
		vec3 worldNormal = texture(normTexture, fragTexCoords).xyz;
		vec3 inColor = texture(colorTexture, fragTexCoords).xyz;
		//float occlusion = texture(ssaoTexture, fragTexCoords).z;
		float occlusion = texture(blurTexture, fragTexCoords).z;
		
		float light = 0, amb = 0.5f;

		vec3 viewLightPos = (V * vec4(lightPos, 1)).xyz;
		vec3 viewEyePos = (V * vec4(eyePos, 1)).xyz;

		float dif, spec, normDotL, normDotH, recvLight;
		vec3 L = normalize(viewLightPos - worldPos);
		vec3 V = normalize(viewEyePos - worldPos);
		vec3 H = normalize(L + V);

		normDotL = dot(worldNormal, L);
		normDotH = dot(worldNormal, H);

		recvLight = normDotL > 0 ? normDotL : 0;
		dif = materialKd * recvLight;
		spec = materialKs * recvLight * pow((normDotH > 0 ? normDotH : 0), materialShininess);

		light = (amb * occlusion) + dif + spec;
		//light = amb  + dif + spec;

		color = vec4(inColor * light, 1);
		//color = vec4(inColor, 1);
	}
}