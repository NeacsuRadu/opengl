#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 position; 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 cameraDir;

in vec3 Normal;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
    vec3 direction = normalize(fragPos - viewPos);
    float doot = dot(direction, cameraDir);
    if (doot > cos(radians(10.0)))
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else 
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
