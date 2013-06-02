#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 normal;
in vec4 position;
in vec3 diffuse;
in vec2 texcoord;

layout(location=2) uniform mat4 normalMatrix;
uniform int hasDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform int hasNormalMap;
uniform sampler2D normalMap;

out vec4 fragData[3];

void main()
{
	vec4 newNormal = normal.xyzw;
	if(hasNormalMap == 1) {
		newNormal = texture2D(normalMap, texcoord).xyzw - vec4(0.5);
		newNormal.w = 0;
		normalize(newNormal);
		newNormal = normalMatrix * newNormal;	
	}
	normalize(newNormal);

	vec3 diffuseColor = diffuse.xyz;
	if(hasDiffuseTexture == 1) {
		diffuseColor = texture2D(diffuseTexture, texcoord).xyz * diffuse.xyz;
	}

    fragData[0] = vec4(diffuseColor.xyz, 2.0);
	fragData[1] = vec4(position.xyz, 1.0);
	fragData[2] = vec4(newNormal.xyz, 0.0);
}
