/*
* Normal shader (model space)
*
*/

#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 normal;

out vec4 fragData[3];

void main()
{
	vec3 newNormal = normal.xyz;
	normalize(newNormal);
    fragData[0] = vec4(1, 0, 0, 0);
	fragData[1] = vec4(0, 0, 0, 1);
	fragData[2] = vec4(newNormal, 0);
}
