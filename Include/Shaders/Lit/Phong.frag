R"(#version 330 core
out vec4 fragColour;

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;
in mat3 TBN;

// Texture of object
uniform sampler2D diffuseMap;
uniform bool useTexture;
uniform vec3 materialSpec;
uniform float shininess;

// Normal mapping
uniform sampler2D normalMap;
uniform bool useNormalTexture;

// Light properties
// max number of lights
const int MAX_NUM_DIR_LIGHTS = 3;
const int MAX_NUM_POINT_LIGHTS = 16;

uniform int numDirLights;
uniform int numPointLights;

// Matieral colour
uniform vec3 diffuseColour;

// various type of light
struct DirLight {
    vec3 direction;
    vec3 baseLight;
};

struct PointLight {
    vec3 position;
    vec3 baseLight;

    float constant;
    float linear;
    float quadratic;
};

uniform DirLight dirLights[MAX_NUM_DIR_LIGHTS];
uniform PointLight pointLights[MAX_NUM_POINT_LIGHTS];
uniform vec3 ambientLight;

uniform bool isDirLight = false;
uniform bool isPointLight = false;
uniform vec3 viewPos;
uniform bool useBlinnPhong;

// shadow mapping properties
uniform sampler2D depthMap;
uniform mat4 lightTransform;

// Calculate lights
vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 colour, vec4 position);
vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour);
float ShadowCalculation(vec4 position, vec3 normal, vec3 lightDirection);
float Calculate_Avg_Dblockreceiver(vec2 projCoords_xy, int AvgTextureSize,int layer);

void main()
{
    vec3 colour = diffuseColour;
    if (useTexture)
    {
        colour = texture(diffuseMap, vertexTextureCoordinate).rgb;
    }

    // get normals
    vec3 normal = normalize(vertexNormal);
    if (useNormalTexture)
    {
        normal = texture(normalMap, vertexTextureCoordinate).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBN * normal);
    }

    // ambient
    vec3 ambient = ambientLight * colour;
    vec3 viewDirection = normalize(viewPos - vertexPos);
    vec3 result = vec3(0.0);

    if(isDirLight)
    {
        for(int i = 0; i < numDirLights; i++)
            result += GetDirLight(dirLights[i], normal, viewDirection, colour, lightTransform*vec4(vertexPos,1.0f));
    }

    if(isPointLight)
    {
        for(int i = 0; i < numPointLights; i++)
            result += GetPointLight(pointLights[i], normal, vertexPos, viewDirection, colour);
    }

    // Final colour
    fragColour = vec4(ambient + result, 1.0);

    // Gamma correction
    float gamma = 2.2;
    fragColour.rgb = pow(fragColour.rgb, vec3(1.0/gamma));
}

vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 colour, vec4 position)
{
    vec3 lightDir = vec3(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec = 0.0;
    if(useBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 4.0f*shininess);
    }
    else
    {
       vec3 reflectDir = reflect(-lightDir, normal);
       spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }

    // combine results
    vec3 diffuse = light.baseLight * diff * colour;
    vec3 specular = light.baseLight * spec * materialSpec;
    float shadow = ShadowCalculation(position, normal, light.direction);
    return (1.0-shadow) * (diffuse + specular);
    //return (diffuse + specular);
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec = 0.0;
    if(useBlinnPhong)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 4.0f*shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 diffuse = light.baseLight * diff * colour * attenuation;
    vec3 specular = light.baseLight * spec * materialSpec * attenuation;

    return (diffuse + specular);
}

float ShadowCalculation(vec4 position, vec3 normal, vec3 lightDirection)
{
    // This gives position in range [-1, 1]
    vec3 coords = position.xyz / position.w;

    // However, depth map range is [0, 1], so transform coords
    coords = coords * 0.5 + 0.5;

    // Closest depth from the lights point of view
    float closestDepth = texture(depthMap, coords.xy).r;

    // The current depth
    float currentDepth = coords.z;

    // To solve shadow acne, subtract a bias
    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);

    float shadow = currentDepth - 0.002 > closestDepth ? 1.0 : 0.0;

    return shadow;
}
)"