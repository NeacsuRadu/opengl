#version 330 core

in vec2 TexCoords;

uniform sampler2D depthMap;

out vec4 FragColor;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float near_plane = 1.0;
    float far_plane = 7.5;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue)), 1.0);
}