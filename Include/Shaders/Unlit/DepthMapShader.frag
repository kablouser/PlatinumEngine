R"(#version 330 core

void main()
{
    // For completeness this is here, but we don't actually colour anything
    gl_FragDepth = gl_FragCoord.z;
}
)"