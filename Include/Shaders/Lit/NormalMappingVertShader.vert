R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;
layout (location = 3) in vec3 inTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPos;
out vec2 vertexTextureCoordinate;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vertexPos = vec3(projection * view * model * vec4(inVertex, 1.0));
    gl_Position = projection * view * model * vec4(inVertex, 1.0);
    vertexTextureCoordinate = inTextureCoordinate;

    vec3 T = normalize(vec3(model * vec4(inTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal, 0.0)));
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * vec3(model * vec4(vertexPos, 1.0));
}
)"