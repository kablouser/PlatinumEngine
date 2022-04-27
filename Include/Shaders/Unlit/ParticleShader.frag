R"(
#version 330 core

out vec4 outColour;
in float life;

uniform float maxLife;
uniform vec4 startColour;
uniform vec4 endColour;
uniform bool useCosineInterpolator;

const float PI = 3.1415926535897932384626433832795;

float CosineInterpolate(float start, float end, float t)
{
   float temp = (1 - cos(t * PI)) / 2;
   return(end * (1 - temp) + start * temp);
}

// Q1*(1-r) + Q2*r
float Lerp(float start, float end, float t)
{
    return end*(1-t) + start * t;
}

void main()
{
    // Calc interpolator value by mapping range to 0-1
    // For clarity, it is laid out like this
    float curMin = 0.0f;
    float curMax = maxLife;
    float targetMin = 0.0f;
    float targetMax = 1.0f;
    float curValue = life;

    float value = (((curValue - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;

    float r;
    float g;
    float b;
    float a;

    if (useCosineInterpolator)
    {
        r = CosineInterpolate(startColour.r, endColour.r, value);
        g = CosineInterpolate(startColour.g, endColour.g, value);
        b = CosineInterpolate(startColour.b, endColour.b, value);
        a = CosineInterpolate(startColour.a, endColour.a, value);
    }
    else
    {
        r = Lerp(startColour.r, endColour.r, value);
        g = Lerp(startColour.g, endColour.g, value);
        b = Lerp(startColour.b, endColour.b, value);
        a = Lerp(startColour.a, endColour.a, value);
    }

    outColour = vec4(r, g, b, a);
}
)"