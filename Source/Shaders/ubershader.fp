#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

uniform sampler2DRect diffuseBuffer;
uniform sampler2DRect specularBuffer;
uniform sampler2DRect positionBuffer;
uniform sampler2DRect normalBuffer;

layout(location=0)uniform int numLights;
layout(location=1)uniform vec3 LightEyespacePositions[20];
layout(location=21)uniform vec3 LightColors[20];
layout(location=41)uniform float LightIntensities[20];

in vec2 texcoord;

out vec4 fragColor;

void main()
{
	vec4 diffuseData = texture2DRect(diffuseBuffer, gl_FragCoord.xy);
	vec4 specularData = texture2DRect(specularBuffer, gl_FragCoord.xy);
	vec4 positionData = texture2DRect(positionBuffer, gl_FragCoord.xy);
	vec4 normalData = texture2DRect(normalBuffer, gl_FragCoord.xy);

	vec3 diffuseColor = diffuseData.xyz;
	vec3 specularColor = specularData.xyz;
	vec3 normal = normalData.xyz;
	vec3 position = positionData.xyz;

	int materialID = int(diffuseData.w);

	vec3 finalColor = vec3(0.0);

	//normal shader
	if(materialID == 0) {
		finalColor = (normal * 0.5) + vec3(0.5);
	//green test shader
	} else if(materialID == 1) {
		finalColor = diffuseColor;
	//lambertian
	} else if(materialID == 2) {
		for (int i = 0; i < numLights; i++) {
			vec3 lightDirection = vec3(LightEyespacePositions[i] - position);
			lightDirection = normalize(lightDirection);
			float ndotl = dot(lightDirection, normal);
			ndotl = max(ndotl, 0);
			finalColor += diffuseColor * LightIntensities[i] * ndotl * LightColors[i];
			//hacky ambient
			finalColor += diffuseColor * LightIntensities[i] * LightColors[i] * 0.1;
		}
	//blinn-phong
	} else if(materialID == 3) {
		float exponent = positionData.w;
		vec3 viewDirection = -normalize(vec3(position));

		for (int i = 0; i < numLights; i++) {
			vec3 lightDirection = normalize(vec3(LightEyespacePositions[i] - position));
			vec3 halfVector = normalize(vec3(lightDirection + viewDirection));

			float ndotl = max(dot(normal, lightDirection), 0.0);
			float ndoth = max(dot(normal, halfVector), 0.0);
			
			if(ndotl > 0.0) {
				//specular
				ndoth = pow(ndoth, exponent);
				finalColor += specularColor * LightIntensities[i] * ndoth * LightColors[i];

				//diffuse
				finalColor += diffuseColor * LightIntensities[i] * ndotl * LightColors[i];
			}
			
			//hacky ambient
			finalColor += diffuseColor * LightIntensities[i] * LightColors[i] * 0.1;
			
			//show normals instead
			//finalColor = (0.5 * normal) + vec3(0.5);	
		}
	}

	fragColor = vec4(finalColor, 1.0);
}
