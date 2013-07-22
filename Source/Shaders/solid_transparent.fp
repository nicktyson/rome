#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

in vec4 normal;
in vec4 position;
in vec2 texcoord;

layout(location=2) uniform mat4 normalMatrix;
uniform vec3 diffuseColor;
uniform float alpha;
uniform int hasDiffuseTexture;
uniform sampler2D diffuseTexture;

uniform int passNumber;

uniform sampler2DRect opaqueDepthBuffer;
uniform sampler2DRect peelDepthBuffer;

out vec4 fragData[1];

void main()
{
	//make sure fragment is between the two depth buffer depths
	float opaqueDepth = texture2DRect(opaqueDepthBuffer, gl_FragCoord.xy).x;
	float previousLayerDepth = texture2DRect(peelDepthBuffer, gl_FragCoord.xy).x;

	if (gl_FragCoord.z >= opaqueDepth) {
		discard;
	}

	if (passNumber != 1 && gl_FragCoord.z <= (previousLayerDepth + 0.0001)) {
		discard;
	}

	//calculate the final color (ie read diffuse texture)
	vec4 outputData = vec4(diffuseColor.xyz, alpha);
	if(hasDiffuseTexture == 1) {
		outputData.xyzw = texture2D(diffuseTexture, texcoord).xyzw * outputData.xyzw;
	}

	if (outputData.w == 0.0) {
		discard;
	}

	fragData[0] = vec4(outputData.x, outputData.y, outputData.z, outputData.w);
	//fragData[0] = vec4(opaqueDepth, opaqueDepth, opaqueDepth, 1.0);
}