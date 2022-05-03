R"(
#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec2 inTextureCoords;
layout (location = 2) in vec4 inPosition;
layout (location = 3) in vec4 inVelocity;
layout (location = 4) in vec2 inTextureIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform bool isSpherical;
uniform vec3 scaleFactors;

// In order to scale appropriately for textures, we need to scale by this ratio
uniform float textureRatio;

out float life;
out vec2 texCoords;
out vec3 position;
out vec3 velocity;
out float scale; // scale for individual particle
out vec2 textureIndex;

void main()
{
    position = inPosition.xyz;
    life = inPosition.w;
    velocity = inVelocity.xyz;
    scale = inVelocity.w;
    texCoords = inTextureCoords;
    textureIndex = inTextureIndex;
    vec3 Position = inPosition.xyz;
    mat4 ModelView = view * model;

    // Get the current scale factor
    float d = sqrt((ModelView[0][0] * ModelView[0][0]) + (ModelView[0][1] * ModelView[0][1]) + (ModelView[0][2] * ModelView[0][2]));

    // Because we are rendering sprites we remove the scaling components but keep scale
    // Cylindrical billboards are fixed in one axis

    // Common changes
    ModelView[0][0] = d * textureRatio * scaleFactors.x;
    ModelView[0][2] = 0;
    ModelView[1][0] = 0;
    ModelView[1][2] = 0;
    ModelView[2][0] = 0;
    ModelView[2][2] = d * 1/textureRatio * scaleFactors.z;

    // Need to adjust y scale always, but may be overwritten to remove rotation if spherical
    ModelView[1][1] *= scaleFactors.y;

    // Spherical reset every axis
    if (isSpherical)
    {
        ModelView[0][1] = 0;
        ModelView[1][1] = d * scaleFactors.y;
        ModelView[2][1] = 0;
    }

    // Final position
    gl_Position = projection * ModelView * vec4((scale * inVertex) + Position, 1.0);
}
)"