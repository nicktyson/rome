#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 normal;
in vec4 position;
in vec3 diffuse;
in vec2 texcoord;

uniform int hasDiffuseTexture;
uniform sampler2D diffuseTexture;

out vec4 fragData[3];

void main()
{
	vec3 newNormal = normal.xyz;
	normalize(newNormal);

	vec3 diffuseColor = diffuse.xyz;
	if(hasDiffuseTexture == 1) {
		diffuseColor = texture2D(diffuseTexture, texcoord).xyz * diffuse.xyz;
	}

    fragData[0] = vec4(diffuseColor.xyz, 2.0);
	fragData[1] = vec4(position.xyz, 1.0);
	fragData[2] = vec4(newNormal, 0.0);
}
