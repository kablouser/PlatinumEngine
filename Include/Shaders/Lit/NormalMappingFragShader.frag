R"(#version 330 core
out vec4 fragColour;

in vec3 vertexPos;
in vec2 vertexTextureCoordinate;
in mat3 TBN;

// Texture and Normal Texture of object
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 materialSpec;
uniform float shininess;

// Light properties
uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// If false, use lightPos as lightDirection
uniform bool isPointLight;

uniform vec3 viewPos;

uniform bool useBlinnPhong;

void main()
{
    vec3 colour = texture(diffuseMap, vertexTextureCoordinate).rgb;

    // ambient
    vec3 ambient = lightAmbient * colour;

    vec3 lightDirection = lightPos;
    if (isPointLight)
    {
        lightDirection = normalize(lightPos - vertexPos);
    }

    vec3 normal = texture(normalMap, vertexTextureCoordinate).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);

    // diffuse
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = lightDiffuse * diff * colour;

    // specular
    vec3 viewDirection = normalize(viewPos - vertexPos);
    float spec = 0.0;
    if(useBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDirection + viewDirection);
        spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDirection, normal);
        spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
    }
    vec3 specular = lightSpecular * (spec * materialSpec);

    // Final colour
    fragColour = vec4(ambient + diffuse + specular, 1.0);
}
)"