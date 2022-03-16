R"(#version 330 core
in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;

out vec4 FragColor;

//struct Material {
//    sampler2D diffuseCoefficient;
//    sampler2D specularCoefficient;
//    float shininess;
//};

struct Light {
    vec3 position;
    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};

uniform vec3 objectColour = vec3(1.0, 1.0, 1.0);
uniform vec3 viewPos;
// uniform Material material;
uniform Light light;

void main()
{
    // ambient
    // vec3 ambientColour = light.ambientStrength * texture(material.diffuseCoefficient, vertexTextureCoordinate).rgb;
    vec3 ambientColour = light.ambientStrength * objectColour;

    // diffuse
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(light.position - vertexPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuseColour = light.diffuseStrength * diff * texture(material.diffuseCoefficient, vertexTextureCoordinate).rgb;
    vec3 diffuseColour = light.diffuseStrength * diff * objectColour;

    // specular
    vec3 viewDir = normalize(viewPos - vertexPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float shininess = 34.0f;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // vec3 specularColour = light.specularStrength * spec * texture(material.specularStrength, vertexTextureCoordinate).rgb;
    vec3 specularColour = light.specularStrength * spec * objectColour;

    vec3 result = ambientColour + diffuseColour + specularColour;
    FragColor = vec4(result, 1.0);
}
)"