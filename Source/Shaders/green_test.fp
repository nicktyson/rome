#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0,1,0,1);
}
