R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec2 inTextureCoords;
layout (location = 2) in vec4 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

out float life;
out vec2 texCoords;
out vec3 position;

void main()
{
    texCoords = inTextureCoords;
    vec3 Position = inPosition.xyz;
    mat4 ModelView = view * model;
    float d = sqrt((ModelView[0][0] * ModelView[0][0]) + (ModelView[0][1] * ModelView[0][1]) + (ModelView[0][2] * ModelView[0][2]));
    // Column 0:
    ModelView[0][0] = d;
//    ModelView[0][1] = 0;
    ModelView[0][2] = 0;

    // Column 1:
    ModelView[1][0] = 0;
//    ModelView[1][1] = d;
    ModelView[1][2] = 0;

    // Column 2:
    ModelView[2][0] = 0;
//    ModelView[2][1] = 0;
    ModelView[2][2] = d;
    gl_Position = projection * ModelView * vec4(inVertex + Position, 1.0);
    position = inPosition.xyz;
    life = inPosition.w;

    // Billboarding cylindrical
//    vec3 _pos = (inVertex + inPosition.xyz);
//    vec3 pos = ((_pos.x) * transpose(view)[0] + _pos.y * vec4(normalize(vec3(0,1,0)), 0.0)).xyz;
//    vec4 position = vec4(pos, 1.0);
//    gl_Position = projection * view * model *  position;
}
)"