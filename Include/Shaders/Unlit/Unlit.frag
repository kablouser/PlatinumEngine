R"(
#version 330 core
in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;

out vec4 outColour;

// various type of light
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// a basic colour of object, texture will be implemented in the future
uniform vec3 objectColour = vec3(1.0, 1.0, 1.0);

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

// camera position
uniform vec3 viewPosition;

// the flags of lights
uniform bool isDirectedLight = false;
uniform bool isPointLight = false;
uniform bool isSpotLight = false;

vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(viewPosition - vertexPos);
    vec3 result = vec3(0.0, 0.0, 0.0);

    // Identify the various types of light sources, then calculate the fragment colour of them
    if(isDirectedLight)
        result += GetDirLight(dirLight, norm, viewDir);
    if(isPointLight)
        result += GetPointLight(pointLight, norm, vertexPos, viewDir);
    if(isSpotLight)
        result += GetSpotLight(spotLight, norm, vertexPos, viewDir);

    outColour = vec4(result, 1.0);
}

vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, lightDir), 0.0), 64.0);

    // combine results
    vec3 ambient = light.ambient * objectColour;
    vec3 diffuse = light.diffuse * diff * objectColour;
    vec3 specular = light.specular * spec * objectColour;

    return (ambient + diffuse + specular);
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * objectColour;
    vec3 diffuse = light.diffuse * diff * objectColour;
    vec3 specular = light.specular * spec * objectColour;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * objectColour;
    vec3 diffuse = light.diffuse * diff * objectColour;
    vec3 specular = light.specular * spec * objectColour;

    return (ambient + diffuse + specular) * attenuation * intensity;
}
)"