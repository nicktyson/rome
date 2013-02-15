/*
* Lambertian Shader
*
*/

uniform float lightsOn;

varying vec3 normal;
varying vec3 position;
varying vec3 lightPosition;

void main()
{
	normalize(normal);
	vec3 lightDirection = lightPosition - position;
	normalize(lightDirection);
	vec3 diffuse = dot(lightDirection, normal);
    gl_FragColor = vec4(diffuse * vec3(1.0), 1.0);
}
