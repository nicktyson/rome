#version 330
#extension GL_ARB_explicit_uniform_location : require

uniform sampler2DRect DiffuseBuffer;

out vec4 fragColor;

void main()
{
	vec3 diffuse = texture2DRect(DiffuseBuffer, gl_FragCoord.xy).xyz;
	fragColor = vec4(diffuse, 1.0);
}