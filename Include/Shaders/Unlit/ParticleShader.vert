R"(
#version 330 core
layout (location = 0) in vec3 inVertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 ModelView = view * model;
    float d = sqrt((ModelView[0][0] * ModelView[0][0]) + (ModelView[0][1] * ModelView[0][1]) + (ModelView[0][2] * ModelView[0][2]));
    // Column 0:
    ModelView[0][0] = d;
    ModelView[0][1] = 0;
    ModelView[0][2] = 0;

    // Column 1:
    ModelView[1][0] = 0;
    ModelView[1][1] = d;
    ModelView[1][2] = 0;

    // Column 2:
    ModelView[2][0] = 0;
    ModelView[2][1] = 0;
    ModelView[2][2] = d;
    gl_Position = projection * ModelView * vec4(inVertex, 1.0);
}
)"