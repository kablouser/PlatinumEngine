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

// Normal mapping
uniform sampler2D normalMap;
uniform bool useNormalTexture;

// Light properties
// max number of lights
const int MAX_NUM_DIR_LIGHTS = 3;
const int MAX_NUM_POINT_LIGHTS = 16;

uniform int numDirLights;
uniform int numPointLights;

// Material properties
uniform vec3 emissive;
uniform vec3 albedo;
uniform float metalness;
uniform float shininess;

const float pi = 3.1415926538;

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

// Calculate lights
vec3 PBR(vec3 LD, vec3 LC, vec3 N, vec3 VD, vec3 colour);

void main()
{
    vec3 colour = albedo;
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
            result += PBR(normalize(-dirLights[i].direction), dirLights[i].baseLight, normal, viewDirection, colour);
    }

    if(isPointLight)
    {
        for(int i = 0; i < numPointLights; i++)
            result += PBR(normalize(pointLights[i].position - vertexPos), pointLights[i].baseLight, normal, viewDirection, colour);
    }

    // Final colour
    fragColour = vec4(emissive + ambient + result, 1.0);

    // Gamma correction
    float gamma = 2.2;
    fragColour.rgb = pow(fragColour.rgb, vec3(1.0/gamma));
}

vec3 PBR(vec3 LD, vec3 LC, vec3 N, vec3 VD, vec3 colour)
{
    vec3 HV = normalize(LD + VD);
    vec3 F0 = (1.0 - metalness) * vec3(0.04, 0.04, 0.04) + metalness * colour;

    // Specular Reflection using Schlick approximation
	vec3 F = F0 + (1.0 - F0) * (pow(1.0 - dot(HV, VD), 5));

	// Diffuse term
	vec3 diffuseTerm = (colour/pi) * (vec3(1,1,1) - F) * (1.0-metalness);

	// Blinn Phong distribution
	float D = ((shininess + 2.0) / (2.0 * pi)) * pow(max(0.0, dot(N, HV) ), shininess);

	// Masking term using Cook-Torrance model
	float G = min(1.0, min( 2 * ((max(0.0, dot(N, HV) ) * max(0.0, dot(N, VD) ))/(dot(VD, HV) ) ), 2 * ((max(0.0, dot(N, HV) ) * max(0.0, dot(N, LD) ))/(dot(VD, HV) ) )) );

	// Specular term
	// sometimes denominator can be 0 so add a small epsilon
	float epsilon = 0.0000001;
	vec3 specularTerm = (D * F * G) / ((4.0 * max(0.0, dot(N, VD) ) * max(0.0, dot(N, LD) ) ) + epsilon );

	return (diffuseTerm + specularTerm) * LC * max(0.0, dot(N, LD));
}
)"