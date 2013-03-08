#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

uniform sampler2DRect diffuseBuffer;
uniform sampler2DRect positionBuffer;
uniform sampler2DRect normalBuffer;

layout(location=0)uniform int numLights;
layout(location=1)uniform vec3 LightEyespacePositions[20];
layout(location=21)uniform vec3 LightColors[20]; 

out vec4 fragColor;

void main()
{
	vec4 diffuseData = texture2DRect(diffuseBuffer, gl_FragCoord.xy);
	vec4 positionData = texture2DRect(positionBuffer, gl_FragCoord.xy);
	vec4 normalData = texture2DRect(normalBuffer, gl_FragCoord.xy);

	vec3 diffuseColor = diffuseData.xyz;
	vec3 normal = normalData.xyz;
	vec3 position = positionData.xyz;

	int materialID = int(diffuseData.w);

	//normal shader
	if(materialID == 0) {
		diffuseColor = (normal * 0.5) + vec3(0.5);
	//lambertian
	} else if(materialID == 2) {
		for (int i = 0; i < numLights; i++) {
			vec3 lightDirection = vec3(LightEyespacePositions[i] - position);
			lightDirection = normalize(lightDirection);
			float ndotl = dot(lightDirection, normal);
			diffuseColor += ndotl * LightColors[i];
		}
	}

	fragColor = vec4(diffuseColor, 1.0);
}
