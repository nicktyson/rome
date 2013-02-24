#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

layout(location=0) uniform sampler2DRect finalBuffer;

out vec4 fragColor;

void main()
{
	vec3 color = texture2DRect(finalBuffer, gl_FragCoord.xy).xyz;
	fragColor = vec4(color, 1.0);
}