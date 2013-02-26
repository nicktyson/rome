#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;

out vec4 fragData[2];

void main()
{
    fragData[0] = vec4(0,1,0,1);
	fragData[1] = vec4(0,0,0,1);
}
