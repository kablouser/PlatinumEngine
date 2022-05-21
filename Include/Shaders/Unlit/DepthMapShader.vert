R"(#version 330

layout (location = 0) in vec3 inPosition;

// Light matrix goes here
uniform mat4 lightTransform;
uniform mat4 model;

void main()
{
    // This shader simply transforms an input vertex into the light space coordinate system
    gl_Position = lightTransform * model * vec4(inPosition, 1.0f);
}
)"