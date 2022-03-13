R"(
#version 460 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;

uniform mat4 model; // projection * view * model
uniform mat4 view;
uniform mat4 projection;


out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexTextureCoordinate;

void main()
{
    vertexPos = vec3(projection * view * model * vec4(inVertex, 1.0));
    vertexNormal = inNormal;
    gl_Position = modelToProjection * vec4(inVertex, 1.0);
    vertexTextureCoordinate = inTextureCoordinate;
}
)"