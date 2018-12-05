#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform sampler2D shadowMap;

in vec3 Normal;
in vec2 TxtCoord;
in vec3 fragPos;
in vec4 FragPosLightSpace;

out vec4 FragColor;

float ShadowCalculations(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    if (currentDepth > closestDepth)
        return 1.0;
    
    return 0.0;
}

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TxtCoord));

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -light.direction), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TxtCoord));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(light.direction, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TxtCoord));

    float shadow = ShadowCalculations(FragPosLightSpace);
    vec3 result = ambient + (diffuse + specular) * (1.0 - shadow); 

    FragColor = vec4(result, 1.0);
}