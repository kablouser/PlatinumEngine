R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position;
out vec3 normal;

void main()
{
    // Set position for rendering on screen
    gl_Position = projection * view * model * vec4(inVertex, 1.0);

    // for calculating reflection and refraction
    position = vec3(model * vec4(inVertex, 1.0));
    normal = mat3(transpose(inverse(model))) * inNormal;
}
)"