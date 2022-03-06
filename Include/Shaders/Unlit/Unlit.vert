R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;

out vec2 outTextureCoordinate;

void main()
{
    gl_Position = vec4(inVertex, 1.0);
    outTextureCoordinate = inTextureCoordinate;
}
)"