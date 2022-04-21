R"(
#version 330 core

// input
in vec3 TexCoords;

// output
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}
)"