R"(
#version 330 core
out vec4 fragColour;

in vec3 position;
in vec3 normal;

// Assumption when false use refraction
uniform bool useReflection;
uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float refractionIndex;

void main()
{
    vec3 I = normalize(position - cameraPos);
    vec3 R = vec3(0.0, 0.0, 0.0);
    if (useReflection)
        R = reflect(I, normalize(normal));
    else
        R = refract(I, normalize(normal), 1.0/refractionIndex);

    fragColour = vec4(texture(skybox, vec3(R.xy, R.z)).rgb, 1.0);
}
)"