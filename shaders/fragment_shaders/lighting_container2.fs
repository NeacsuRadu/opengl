#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float innerCos;
    float outerCos;
};

uniform Material material;
uniform Light light;
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform sampler2D emission;

in vec3 Normal;
in vec2 TxtCoord;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
    float len = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + len * light.linear + len * len * light.quadratic);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TxtCoord));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TxtCoord));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TxtCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular; 

    vec3 direction = normalize(fragPos - spotLight.position);
    float theta = dot(direction, spotLight.direction);
    if (theta > spotLight.outerCos)
    {
        vec3 result2 = ambient;
        float eps = spotLight.innerCos - spotLight.outerCos;
        float intensity = clamp((theta - spotLight.outerCos) / eps, 0.0, 1.0);

        float diff = max(dot(direction, -norm), 0.0);
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TxtCoord));

        result2 += diffuse * intensity;
        result = max(result, result2);
    }
    FragColor = vec4(result, 1.0);
}