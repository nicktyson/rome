/*
* Normal shader (model space)
*
*/

#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;

out vec4 fragData[2];

void main()
{
	vec3 newNormal = normal;
	normalize(newNormal);
    fragData[0] = vec4(1,0,0,0);
	fragData[1] = vec4(newNormal, 1);
}
