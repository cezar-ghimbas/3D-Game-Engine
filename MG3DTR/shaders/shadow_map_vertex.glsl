#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = lightSpaceMatrix * model_matrix * vec4(position, 1);
}
