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
out mat3 TBN;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vertexPos = vec3(model * vec4(inVertex, 1.0));
    vertexTextureCoordinate = inTextureCoordinate;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 _TBN = transpose(mat3(T, B, N));
    TangentLightPos = _TBN * lightPos;
    TangentViewPos  = _TBN * viewPos;
    TangentFragPos  = _TBN * vertexPos;
    TBN = _TBN;
    gl_Position = projection * view * model * vec4(inVertex, 1.0);
}
)"