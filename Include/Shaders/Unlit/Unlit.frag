R"(
#version 330 core
in vec2 inTextureCoordinate;

out vec4 outColor;

uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform bool isTextureEnabled = false;
uniform sampler2D sampleTexture;

void main()
{
    if (isTextureEnabled)
    {
        outColor = texture(sampleTexture, inTextureCoordinate);
    }
    else
    {
        outColor = vec4(color, 1.0);
    }
}
)"