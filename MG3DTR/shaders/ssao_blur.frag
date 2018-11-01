#version 330 core
layout(location = 0) out vec3 ssaoBlur;

in vec2 fragTexCoords;

uniform sampler2D ssaoTexture;

void main()
{
   vec3 color = vec3(0, 0, 0);
   vec2 texSize = 1 / vec2(textureSize(ssaoTexture, 0));
   vec2 tc;

    for (float i = -1.5 ; i <= 1.5; i++) {
        for (float j = -1.5 ; j <= 1.5; j++) {
            tc.x = fragTexCoords.x + float(i) * texSize.x;
            tc.y = fragTexCoords.y + float(j) * texSize.y;
            color += texture(ssaoTexture, tc).xyz;
        }
    }

	color /= 16.0;

    ssaoBlur = color;
}