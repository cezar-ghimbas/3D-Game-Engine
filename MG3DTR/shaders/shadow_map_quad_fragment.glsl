#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
    float d = texture(screenTexture, TexCoords).r;
    color = vec4(vec3(d), 1.0);
    //color = vec4(1, 0, 0, 1);
}
