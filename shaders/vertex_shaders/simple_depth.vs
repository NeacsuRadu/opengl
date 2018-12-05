#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 lightspace_projection;

void main()
{
    gl_Position = lightspace_projection * model * vec4(position, 1.0);
}