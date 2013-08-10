#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

const float PI = 3.14159265358979323846264;

uniform sampler2DRect diffuseBuffer;
uniform sampler2DRect specularBuffer;
uniform sampler2DRect positionBuffer;
uniform sampler2DRect normalBuffer;

uniform samplerCube environmentCubeMap;

layout(location=0)uniform int numLights;
layout(location=1)uniform vec3 LightEyespacePositions[20];
layout(location=21)uniform vec3 LightColors[20];
layout(location=41)uniform float LightIntensities[20];

uniform mat4 eyeToWorldNormalMatrix;

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

	vec3 worldNormal = (eyeToWorldNormalMatrix * vec4(normal, 0.0)).xyz;

	//normal shader
	if(materialID == 0) {
		finalColor = (normal * 0.5) + vec3(0.5);
	//green test shader or solid diffuse
	} else if(materialID == 1 || materialID == 5) {
		finalColor = diffuseColor;
	//lambertian
	} else if(materialID == 2) {
		for (int i = 0; i < numLights; i++) {
			vec3 lightDirection = vec3(LightEyespacePositions[i] - position);
			lightDirection = normalize(lightDirection);
			float ndotl = dot(lightDirection, normal);
			ndotl = max(ndotl, 0);
			finalColor += diffuseColor * LightIntensities[i] * ndotl * LightColors[i];
			//hacky per-light ambient
			//finalColor += diffuseColor * LightIntensities[i] * LightColors[i] * 0.1;
		}
		// environment lighting for ambient
		finalColor += diffuseColor * texture(environmentCubeMap, worldNormal).xyz * 0.1;
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
			
			//hacky per-light ambient
			//finalColor += diffuseColor * LightIntensities[i] * LightColors[i] * 0.1;
			
			//show normals instead
			//finalColor = (0.5 * normal) + vec3(0.5);	
		}
		// environment lighting for ambient
		finalColor += diffuseColor * texture(environmentCubeMap, worldNormal).xyz * 0.1;
	//Cook-Torr
	} else if(materialID == 4) {
		float ctM = positionData.w;
		float ctN = normalData.w;
		vec3 viewDirection = -normalize(vec3(position));

		for (int i = 0; i < numLights; i++) {
			vec3 lightDirection = normalize(vec3(LightEyespacePositions[i] - position));
			vec3 halfVector = normalize(vec3(lightDirection + viewDirection));

			float NdotL = max(dot(normal, lightDirection), 0.0);
			float NdotH = max(dot(normal, halfVector), 0.0);
			float VdotH = max(dot(viewDirection, halfVector), 0.0);
			float NdotV = max(dot(normal, viewDirection), 0.0);

			if (NdotL > 0.0 && NdotH > 0.0 && VdotH > 0.0 && NdotV > 0.0) {
				//specular
				float alpha = acos(NdotH);
				float Rf = pow((ctN - 1)/(ctN + 1), 2);

				float F = Rf + (1.0 - Rf) * pow(1 - NdotV, 5); // or NdotL?
				float D = 1.0 / (4 * pow(ctM, 2) * pow(cos(alpha), 4)) * exp(-pow(tan(alpha) / ctM, 2));
				float G = min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH);
				G = min(G, 1.0);

				finalColor += (specularColor * LightIntensities[i] * LightColors[i] * F * D * G) / (PI * NdotL * NdotV);
				
				//diffuse
				finalColor += diffuseColor * LightIntensities[i] * NdotL * LightColors[i];
			}

			//hacky per-light ambient
			//finalColor += diffuseColor * LightIntensities[i] * LightColors[i] * 0.1;
		}
		// environment lighting for ambient
		finalColor += diffuseColor * texture(environmentCubeMap, worldNormal).xyz * 0.1;
	}

	fragColor = vec4(finalColor, 1.0);
}
