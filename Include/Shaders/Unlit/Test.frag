R"(
#version 330 core
out vec4 outColour;

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;

uniform sampler2D ourTexture;
uniform sampler2D normalMap;
uniform bool useNormalMap = false;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 color = vec3(1.0, 1.0, 0.0);

void main()
{
    vec3 objectColour = texture(ourTexture, vertexTextureCoordinate).rgb;
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(vertexNormal);
    if (useNormalMap)
    {
        norm = texture(normalMap, vertexTextureCoordinate).xyz;
        norm = normalize(norm * 2.0 - 1.0);
    }
    vec3 lightDir = normalize(lightPos - vertexPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vertexPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColour;
    outColour = vec4(result, 1.0);
}
)"