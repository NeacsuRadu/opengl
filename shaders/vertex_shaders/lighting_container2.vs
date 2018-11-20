#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 txtCoords;

out vec3 Normal;
out vec2 TxtCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    Normal = normal;
    TxtCoord = txtCoords;
    fragPos =  vec3(model * vec4(position, 1.0));
}