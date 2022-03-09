R"(
#version 460 core
in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;

out vec4 outColour;

uniform vec3 objectColour = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 viewPosition;

uniform bool isTextureEnabled = false;
uniform sampler2D sampleTexture;

void main()
{
    if (isTextureEnabled)
    {
        outColour = texture(sampleTexture, vertexTextureCoordinate);
    }
    else
    {
        // ambient
        float ambientStrength = 0.1;
        vec3 ambientColour = ambientStrength * lightColour;

        // diffuse
        vec3 norm = normalize(vertexNormal);
        vec3 lightDir = normalize(lightPosition - vertexPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuseColour = diff * lightColour;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPosition - vertexPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float specularCoefficient = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
        vec3 specularColour = specularCoefficient* specularStrength * lightColour;

        vec3 result = (ambientColour + diffuseColour + specularColour) * objectColour;
        outColour = vec4(result, 1.0);
    }
}
)"