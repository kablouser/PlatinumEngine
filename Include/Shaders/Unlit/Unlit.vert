R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;
layout (location = 3) in ivec4 trackIDs;
layout (location = 4) in vec4 weights;


const int MAX_BONES = 300;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isAnimationDisplay;
uniform mat4 tracks[MAX_BONES];

out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexTextureCoordinate;

void main()
{

    if(isAnimationDisplay)
    {
        mat4 animationTransform = tracks[trackIDs[0]] * weights[0];
        animationTransform += tracks[trackIDs[1]] * weights[1];
        animationTransform += tracks[trackIDs[2]] * weights[2];
        animationTransform += tracks[trackIDs[3]] * weights[3];

        vertexPos = vec3(projection * view * model * animationTransform * vec4(inVertex, 1.0));
        gl_Position = projection * view * model * animationTransform * vec4(inVertex, 1.0);
    }
    else
    {
        vertexPos = vec3(projection * view * model * vec4(inVertex, 1.0));
        gl_Position = projection * view * model * vec4(inVertex, 1.0);
    }

    vertexNormal = inNormal;
    vertexTextureCoordinate = inTextureCoordinate;
}
)"