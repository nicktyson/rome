/*
* Normal shader (model space)
*
*/

#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;

out vec4 fragColor;

void main()
{
	normalize(normal);
    fragColor = vec4(vec3(0.5) + normal * 0.5, 1.0);
}
