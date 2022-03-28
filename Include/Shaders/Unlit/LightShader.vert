R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inVertex, 1.0);
}
)"