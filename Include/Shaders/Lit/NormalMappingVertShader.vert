R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPos;
out vec2 vertexTextureCoordinate;
out mat3 TBN;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vertexPos = vec3(model * vec4(inVertex, 1.0));
    vertexTextureCoordinate = inTextureCoordinate;

    vec3 T = normalize(vec3(model * vec4(inTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(inBiTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal, 0.0)));
    TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(inVertex, 1.0);
}
)"