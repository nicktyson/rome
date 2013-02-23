#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;

out vec4 fragColor;

void main()
{
    gl_FragData[0] = vec4(0,1,0,1);
}
