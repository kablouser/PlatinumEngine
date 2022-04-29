R"(
#version 330 core

out vec4 outColour;
in vec3 position;
in float life;
in vec2 texCoords;
in vec3 velocity;
in float scale;
in vec2 textureIndex;

uniform float maxLife;

// It's assumed at time maxLife, frag should be startColour
// at time 0, frag should be endColour
uniform vec4 StartColour;
uniform vec4 EndColour;

// Texture handling
uniform bool useTexture;
uniform sampler2D sampleTexture;
uniform float textureWidth;
uniform float textureHeight;
uniform float spriteWidth;
uniform float spriteHeight;

// Determine what to shade by
uniform bool useShadeByLife;
uniform bool useShadeByPosition;
uniform bool useShadeBySpeed;
uniform bool useShadeBySize;
uniform float minVal;
uniform float maxVal;

// Bezier functions
vec2 Lerp(vec2 start, vec2 end, float t)
{
    return end + (start - end) * t;
}

vec2 QuadCurve(vec2 start, vec2 P2, vec2 end, float t)
{
    vec2 p0 = Lerp(start, P2, t);
    vec2 p1 = Lerp(P2, end, t);
    return Lerp(p0, p1, t);
}

vec2 CubicCurve(vec2 start, vec2 P2, vec2 P3, vec2 end, float t)
{
    vec2 p0 = QuadCurve(start, P2, P3, t);
    vec2 p1 = QuadCurve(P3, P2, end, t);
    return Lerp(p0, p1, t);
}

float LerpChannel(float start, float end, float t)
{
    return start + (end - start) * t;
}

vec4 LerpColour(vec4 startColour, vec4 endColour, float t)
{
    return vec4(LerpChannel(startColour.r, endColour.r, t),
                LerpChannel(startColour.g, endColour.g, t),
                LerpChannel(startColour.b, endColour.b, t),
                LerpChannel(startColour.a, endColour.a, t)
                );
}

void main()
{
    // TODO: Modulate with colour
    if (useTexture)
    {
        // Compute uv coords for this particle using indices to index into texture atlas
        vec2 coords = vec2(((texCoords.x * spriteWidth) + (textureIndex.x*spriteWidth)) / textureWidth,
                           ((texCoords.y * spriteHeight) + (textureIndex.y*spriteHeight)) / textureHeight);
        outColour = texture(sampleTexture, coords).rgba;
        return;
    }

    // Calc interpolator value by mapping range to 0-1
    float targetMin = 0.0f;
    float targetMax = 1.0f;

    float curMin = minVal;
    float curMax = maxVal;
    float curValue;

    if (useShadeByLife)
    {
        curValue = maxLife - life;
    }
    if (useShadeByPosition)
    {
        curValue = distance(vec3(0,0,0), position);
    }
    if (useShadeBySpeed)
    {
        curValue = length(velocity);
    }
    if (useShadeBySize)
    {
        curValue = scale;
    }

    float t = (((curValue - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;

    if (curValue < curMin)
        t = 0.0f;
    if (curValue > curMax)
        t = 1.0f;

    outColour = LerpColour(StartColour, EndColour, t);
}
)"