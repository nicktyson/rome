#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location=0) uniform sampler2DRect diffuseBuffer;

out vec4 fragColor;

void main()
{
	vec3 diffuse = texture2DRect(diffuseBuffer, gl_FragCoord.xy).xyz;
	fragColor = vec4(diffuse, 1.0);
}