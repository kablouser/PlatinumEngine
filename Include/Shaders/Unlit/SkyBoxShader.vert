R"(
#version 330 core

// input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 b;
layout (location = 2) in vec2 c;

// output
out vec3 TexCoords;

// uniform
uniform mat4 view;
uniform mat4 projection;

void main()
{

    TexCoords = aPos;

    gl_Position = projection * view * vec4(aPos, 1.0);

}
)"