#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

uniform sampler2DRect diffuseBuffer;
uniform sampler2DRect normalBuffer;

out vec4 fragColor;

void main()
{
	vec4 diffuseData = texture2DRect(diffuseBuffer, gl_FragCoord.xy);
	vec4 normalData = texture2DRect(normalBuffer, gl_FragCoord.xy);

	vec3 diffuseColor = diffuseData.xyz;
	vec3 normal = normalData.xyz;

	int materialID = int(diffuseData.w);

	if(materialID == 0) {

		diffuseColor = (normal * 0.5) + vec3(0.5);
	}

	fragColor = vec4(diffuseColor, 1.0);
}
