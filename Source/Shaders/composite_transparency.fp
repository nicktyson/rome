#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

uniform sampler2DRect opaqueBuffer;
uniform sampler2DRect colorBuffer1;
uniform sampler2DRect colorBuffer2;
uniform sampler2DRect colorBuffer3;
uniform sampler2DRect colorBuffer4;

in vec2 texcoord;

out vec4 fragColor;

void main()
{
	vec3 opaqueColor = texture2DRect(opaqueBuffer, gl_FragCoord.xy).xyz;

	float alpha1 = texture2DRect(colorBuffer1, gl_FragCoord.xy).w;
	float alpha2 = texture2DRect(colorBuffer2, gl_FragCoord.xy).w;
	float alpha3 = texture2DRect(colorBuffer3, gl_FragCoord.xy).w;
	float alpha4 = texture2DRect(colorBuffer4, gl_FragCoord.xy).w;

	vec3 tcolor1 = texture2DRect(colorBuffer1, gl_FragCoord.xy).xyz;
	vec3 tcolor2 = texture2DRect(colorBuffer2, gl_FragCoord.xy).xyz;
	vec3 tcolor3 = texture2DRect(colorBuffer3, gl_FragCoord.xy).xyz;
	vec3 tcolor4 = texture2DRect(colorBuffer4, gl_FragCoord.xy).xyz;

	//from back to front
	vec3 color = (1.0 - alpha4) * opaqueColor + alpha4 * tcolor4;
	color = (1.0 - alpha3) * color + alpha3 * tcolor3;
	color = (1.0 - alpha2) * color + alpha2 * tcolor2;
	color = (1.0 - alpha1) * color + alpha1 * tcolor1;
	//vec3 color = tcolor3;

	fragColor = vec4(color, 1.0);
}