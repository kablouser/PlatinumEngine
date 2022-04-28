R"(
#version 330 core

out vec4 outColour;
in float life;

uniform float maxLife;

// It's assumed at time maxLife, frag should be startColour
// at time 0, frag should be endColour
uniform vec4 startColour;
uniform vec4 endColour;

// Texture handling
//uniform bool useTexture
//uniform sampler2D sampleTexture;

// For bezier curve, control point locations
// [time][colour]
uniform vec2 P2r;
uniform vec2 P2g;
uniform vec2 P2b;
uniform vec2 P2a;
uniform vec2 P3r;
uniform vec2 P3g;
uniform vec2 P3b;
uniform vec2 P3a;

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

void main()
{
//    if (useTexture)
//    {
//        outColour = texture(sampleTexture, vertexTextureCoordinate).rgb;
//        return;
//    }


    // Calc interpolator value by mapping range to 0-1
    // For clarity, it is laid out like this
    float curMin = 0.0f;
    float curMax = maxLife;
    float targetMin = 0.0f;
    float targetMax = 1.0f;
    float curValue = life;
    float t = (((curValue - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;

//    // Also need to convert P2 time and P3 time
//    float P2rTime = (((P2r[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P2gTime = (((P2g[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P2bTime = (((P2b[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P2aTime = (((P2a[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//
//    float P3rTime = (((P3r[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P3gTime = (((P3g[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P3bTime = (((P3b[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;
//    float P3aTime = (((P3a[0] - curMin) * (targetMax - targetMin)) / (curMax - curMin)) + targetMin;

    // Values for outColour
    float r;
    float g;
    float b;
    float a;

    r = CubicCurve(vec2(1, startColour.r), P2r, P3r, vec2(0, endColour.r), t)[1];
    g = CubicCurve(vec2(1, startColour.g), P2g, P3g, vec2(0, endColour.g), t)[1];
    b = CubicCurve(vec2(1, startColour.b), P2b, P3b, vec2(0, endColour.b), t)[1];
    a = CubicCurve(vec2(1, startColour.a), P2a, P3a, vec2(0, endColour.a), t)[1];

//    r = CubicCurve(vec2(1, startColour.r), vec2(P2rTime, P2r.y), vec2(P3rTime, P3r.y), vec2(0, endColour.r), t)[1];
//    g = CubicCurve(vec2(1, startColour.g), vec2(P2gTime, P2g.y), vec2(P3gTime, P3g.y), vec2(0, endColour.g), t)[1];
//    b = CubicCurve(vec2(1, startColour.b), vec2(P2bTime, P2b.y), vec2(P3bTime, P3b.y), vec2(0, endColour.b), t)[1];
//    a = CubicCurve(vec2(1, startColour.a), vec2(P2aTime, P2a.y), vec2(P3aTime, P3a.y), vec2(0, endColour.a), t)[1];

    outColour = vec4(r, g, b, a);
}
)"