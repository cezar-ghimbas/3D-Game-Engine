#version 330 core
layout(location = 0) out vec3 ssao;

in vec2 fragTexCoords;
out vec4 color;

uniform sampler2D posTexture;
uniform sampler2D noiseTexture;
uniform sampler2D normTexture;

const int SIZE = 64;
uniform vec3 sampleOffsets[SIZE];

uniform mat4 P, V;


void main()
{

	vec2 scale = vec2(1000.0 / 4.0, 800.0 / 4.0);
	//vec2 scale = vec2(600.0 / 4.0, 800.0 / 4.0);
    vec3 pos = texture(posTexture, fragTexCoords).xyz;
	vec3 normal = texture(normTexture, fragTexCoords).xyz;
	vec3 random = texture(noiseTexture, fragTexCoords * scale).xyz;



	vec3 tangent = (random - normal * dot(normal, random));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);

    float AO = 0.0;

    for (int i = 0 ; i < SIZE ; i++) {
        vec3 sample = tbn * sampleOffsets[i];
		sample = pos + sample;

        vec4 offset = vec4(sample, 1.0);
        offset = P * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + vec2(0.5);
            
        float sampleDepth = texture(posTexture, offset.xy).z;

		if ((sampleDepth >= sample.z + 0.25f) && (sampleDepth - sample.z < 1.5f)) {
			AO += 1.0f;
		}
    }

    AO = 1.0 - AO / SIZE;
 
    ssao = vec3(pow(AO, 7.0));

	/*if (sampleOffsets[SIZE - 1].x == -0.944386)
		ssao = 	vec3(1);
	else
		ssao = vec3(0);*/
}