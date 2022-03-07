R"(
#version 330 core
in vec2 inTextureCoordinate;

out vec4 outColor;

uniform vec3 color = vec3(1.0, 1.0, 1.0);

void main()
{
    outColor = vec4(color, 1.0);
}
)"