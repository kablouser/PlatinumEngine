R"(
#version 330 core

out vec4 outColour;
in float life;

void main()
{
    float value = 1.0 / life;
    outColour = vec4(value, 1 - value, 0, value);
    //outColour = vec4(1.0f);
}
)"