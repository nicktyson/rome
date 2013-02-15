/*
* Normal shader (model space)
*
*/

varying vec3 normal;

void main()
{
	normalize(normal);
    gl_FragColor = vec4(vec3(0.5) + normal * 0.5, 1.0);
}
