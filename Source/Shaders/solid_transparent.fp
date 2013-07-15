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

uniform sampler2DRect opaqueDepthBuffer;
uniform sampler2DRect peelDepthBuffer;

out vec4 fragData[1];

void main()
{
	//make sure fragment is between the two depth buffer depths
	float opaqueDepth = texture2DRect(opaqueDepthBuffer, gl_FragCoord.xy).x;
	float previousLayerDepth = texture2DRect(peelDepthBuffer, gl_FragCoord.xy).x;

	if (gl_FragCoord.z >= opaqueDepth || gl_FragCoord.z <= (previousLayerDepth + 0.0001)) {
		discard;
	}

	//calculate the final color (ie read diffuse texture)
	vec3 diffuse = diffuseColor.xyz;
	if(hasDiffuseTexture == 1) {
		diffuse = texture2D(diffuseTexture, texcoord).xyz * diffuseColor.xyz;
	}

	fragData[0] = vec4(diffuseColor.x, diffuseColor.y, diffuseColor.z, alpha);
}