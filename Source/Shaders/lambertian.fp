#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 normal;
in vec4 position;

out vec4 fragColor;

void main()
{
	vec3 newnormal = vec3(normal);
	vec3 lightPosition = vec3(2.0, 2.0, 2.0);
	vec3 lightDirection = vec3(lightPosition.xyz - position.xyz);
	normalize(lightDirection);
	normalize(newnormal);
	float ndotl = dot(lightDirection, newnormal);
	vec3 diffuse = vec3(1.0, 0.0, 0.0);
	diffuse = diffuse * ndotl;
    fragColor = vec4(diffuse, 1.0);
}
