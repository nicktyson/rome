#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 normal;
in vec4 position;
in vec3 diffuse;

layout(location=4) uniform float exponent;

out vec4 fragData[3];

void main()
{
	vec3 newNormal = normal.xyz;
	normalize(newNormal);
    fragData[0] = vec4(diffuse.xyz, 3.0);
	fragData[1] = vec4(position.xyz, exponent);
	fragData[2] = vec4(newNormal, 0.0);
}
