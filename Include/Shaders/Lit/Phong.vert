R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;
layout (location = 5) in vec4 trackIDs;
layout (location = 6) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 128;
uniform bool isAnimationDisplay;
uniform bool isAnimationAttachmentDisplay;
uniform mat4 tracks[MAX_BONES];

out vec3 vertexPos;
out vec3 vertexNormal;
out vec2 vertexTextureCoordinate;
out mat3 TBN;

void main()
{
    mat4 animationTransform = mat4(1.0);

    if(isAnimationDisplay)
    {
         animationTransform  = tracks[int(trackIDs[0])] * weights[0];
         animationTransform += tracks[int(trackIDs[1])] * weights[1];
         animationTransform += tracks[int(trackIDs[2])] * weights[2];
         animationTransform += tracks[int(trackIDs[3])] * weights[3];
    }

    if(isAnimationAttachmentDisplay)
    {
        animationTransform =  tracks[0];
    }

    gl_Position = projection * view * model * animationTransform * vec4(inVertex, 1.0);
//    vertexPos = vec3(gl_Position);
    mat4 animwmod = model * animationTransform;
    vertexPos = vec3(animwmod * vec4(inVertex, 1.0));
    vertexNormal = transpose(inverse(mat3(animwmod))) * inNormal;
    vertexTextureCoordinate = inTextureCoordinate;

    // For normal mapping calc TBN matrix
    vec3 T = normalize(vec3(animwmod * vec4(inTangent, 0.0)));
    vec3 B = normalize(vec3(animwmod * vec4(inBiTangent, 0.0)));
    vec3 N = normalize(vec3(animwmod * vec4(inNormal, 0.0)));
    TBN = mat3(T, B, N);
}
)"