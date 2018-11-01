#version 330
layout(location = 0) out vec4 out_color;

in vec3 world_pos;
in vec3 world_normal;

//uniform sampler2D screenTexture;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;
uniform vec3 color;
uniform int use_light;
uniform samplerCube shadowMap;
uniform float farPlane;

vec3 sampleOffsetDirections[20] = vec3[] (
	vec3(1, 1, 1), vec3(1, -1, 1), vec3 (-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float ShadowCalculation(vec3 lightDir, vec3 normal)
{
	vec3 lightToFrag = world_pos - light_position;
	float closestDepth = texture(shadowMap, lightToFrag).r;
	closestDepth *= farPlane;
	float currentDepth = length(lightToFrag);
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

float ShadowCalculationPCF(vec3 lightDir, vec3 normal)
{
	vec3 lightToFrag = world_pos - light_position;
	float currentDepth = length(lightToFrag);
	float bias = 0.05;

	float shadow = 0.0;
	float samples = 4.0;
	float offset = 0.1;
	for (float x = -offset; x < offset; x+= offset / (samples * 0.5)) {
		for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
			for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
				float closestDepth = texture(shadowMap, lightToFrag + vec3(x, y, z)).r;
				closestDepth *= farPlane;
				if (currentDepth - bias > closestDepth) {
					shadow += 1.0;
				}
			}
		}
	}
	shadow /= (samples * samples * samples);

    return shadow;
}

float ShadowCalculationPCFOptimized(vec3 lightDir, vec3 normal)
{
	vec3 lightToFrag = world_pos - light_position;
	float currentDepth = length(lightToFrag);
	float bias = 0.15;
	int samples = 20;
	float shadow = 0.0;
	float viewDistance = length(eye_position - world_pos);
	//float diskRad = 0.05;
	float diskRad = (1.0 + (viewDistance / farPlane)) / 25.0;
	for (int i = 0; i < samples; ++i) {
		float closestDepth = texture(shadowMap, lightToFrag + sampleOffsetDirections[i] * diskRad).r;
		closestDepth *= farPlane;
		if (currentDepth - bias > closestDepth) {
			shadow += 1.0;
		}
	}
	shadow /= float(samples);

    return shadow;
}

void main()
{           
    vec3 normal = normalize(world_normal);
    vec3 lightColor = vec3(1.0);
    // Ambient
    vec3 ambient = 0.15 * color;
    // Diffuse
    vec3 lightDir = normalize(light_position - world_pos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(eye_position - world_pos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // Calculate shadow
    float shadow = ShadowCalculationPCFOptimized(lightDir, normal);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    //vec3 lighting = (ambient + (diffuse + specular)) * color;
    
    out_color = vec4(lighting, 1.0f);
}

