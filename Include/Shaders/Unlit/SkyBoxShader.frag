R"(
#version 330 core

// input
in vec3 TexCoords;

// output
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 colour = vec3(1.0f,1.0f,1.0f);

void main()
{
    FragColor = vec4(colour,1) * texture(skybox, TexCoords);
}
)"