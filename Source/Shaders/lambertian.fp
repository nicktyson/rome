#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 normal;
in vec4 position;
in vec3 diffuse;
in vec2 texcoord;
in vec3 eyeSpaceTangent;
in vec3 eyeSpaceBitangent;

layout(location=2) uniform mat4 normalMatrix;
uniform int hasDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform int hasNormalMap;
uniform sampler2D normalMap;

out vec4 fragData[4];

void main()
{
	vec3 newNormal = normal.xyz;
	newNormal = normalize(newNormal);

	if (hasNormalMap == 1) {
		vec3 eyeTangent = normalize(eyeSpaceTangent);
		vec3 eyeBitangent = normalize(eyeSpaceBitangent);
		
		//reorthagonalize, since the interpolation probably messed it up
		vec3 tangent = normalize(eyeTangent - dot(eyeTangent, newNormal)*newNormal);
		vec3 bitangent = normalize(cross(newNormal, tangent));

		//find the handedness of the interpolated vectors
		float sig = dot(cross(newNormal, eyeTangent), eyeBitangent);
		sig = sign(sig);

		//find the handedness of the reorthagonalized vectors
		float sig2 = dot(cross(newNormal, tangent), bitangent);
		sig2 = sign(sig2);

		//make sure the reorthagonalized basis has the same handedness as the interpolated one
		//this should only happen if the reorthagonalization messes up handedness, which may or may not be possible
		if (sig != sig2) {
			bitangent *= -1.0;
		}

		mat3 switcher = mat3(tangent, bitangent, newNormal);

		newNormal = texture2D(normalMap, texcoord).xyz - vec3(0.5, 0.5, 0.5);
		newNormal = normalize(newNormal);
		newNormal = switcher * newNormal;
		newNormal = normalize(newNormal);
	}

	vec3 diffuseColor = diffuse.xyz;
	if(hasDiffuseTexture == 1) {
		diffuseColor = texture2D(diffuseTexture, texcoord).xyz * diffuse.xyz;
	}

	//materialID = 2.0
    fragData[0] = vec4(diffuseColor.xyz, 2.0);
	fragData[1] = vec4(1.0, 1.0, 1.0, 1.0);
	fragData[2] = vec4(position.xyz, 1.0);
	fragData[3] = vec4(newNormal.xyz, 0.0);
}
