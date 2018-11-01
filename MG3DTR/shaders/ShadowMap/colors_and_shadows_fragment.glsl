#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

uniform int use_light;

//uniform vec3 color;

in vec3 world_pos;
in vec3 world_normal;

in vec2 texCoords;

//uniform sampler2D screenTexture;
in vec4 light_space;

uniform sampler2D shadowMap;
uniform sampler2D objTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    //float bias = 0.005;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
    	for (int y = -1; y <= 1; ++y) {
    		float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
    		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    	}
    }
    
    shadow /= 9.0;
    
    if (projCoords.z > 1.0) {
    	shadow = 0.0;
    }

    return shadow;
}

void main()
{           
	vec3 color = texture(objTexture, texCoords).xyz;

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
    float shadow = ShadowCalculation(light_space, lightDir, normal);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    //vec3 lighting = (ambient + (diffuse + specular)) * color;
    
    out_color = vec4(lighting, 1.0f);
}

