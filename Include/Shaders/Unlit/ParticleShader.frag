R"(
#version 330 core

out vec4 outColour;
in float life;

uniform float maxLife;
uniform vec4 startColour;
uniform vec4 endColour;

void main()
{
    // Q1*(1-r) + Q2*r
    float value = (1.0 / maxLife) * life;
    float r = endColour.r*(1-value) + startColour.r * value;
    float g = endColour.g*(1-value) + startColour.g * value;
    float b = endColour.b*(1-value) + startColour.b * value;
    float a = endColour.a*(1-value) + startColour.a * value;
    outColour = vec4(r, g, b, a);
}
)"