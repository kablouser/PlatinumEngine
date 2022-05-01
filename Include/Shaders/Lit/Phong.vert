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
out vec3 vertexNormal;
out vec2 vertexTextureCoordinate;
out mat3 TBN;

void main()
{
    gl_Position = projection * view * model * vec4(inVertex, 1.0);
    vertexPos = vec3(gl_Position);
    mat4 viewmod = view * model;
    vertexNormal = transpose(inverse(mat3(viewmod))) * inNormal;
    vertexTextureCoordinate = inTextureCoordinate;

    // For normal mapping calc TBN matrix
    vec3 T = normalize(vec3(model * vec4(inTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(inBiTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal, 0.0)));
    TBN = mat3(T, B, N);
}
)"