#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_texture_rectangle : require

//the algorithm used here is from CS 5625, Cornell; the code itself is mine

layout(location=0) uniform sampler2DRect finalBuffer;

out vec4 fragColor;

void main()
{
	float KERNEL_VARIANCE = 15.0;
	int KERNEL_WIDTH = 3;
	float THRESHOLD = 0.9;

	float brightnessSum = 0.0;
	float weightSum = 0.0; 

	for (int i = -KERNEL_WIDTH; i < KERNEL_WIDTH + 1; i++) { 
		for (int j = -KERNEL_WIDTH; j < KERNEL_WIDTH + 1; j++) { 
			vec2 coords = vec2(gl_FragCoord.x + i, gl_FragCoord.y + j);		 	
			vec3 fragcolor = texture2DRect(finalBuffer, coords).xyz; 
			float thresholdedValue = 0.299*fragcolor.x + 0.587*fragcolor.y + 0.114*fragcolor.z; 
			thresholdedValue = step(THRESHOLD, thresholdedValue); 
			 
			float weight = exp(-(pow(float(abs(i)), 2.0) + pow(float(abs(j)), 2.0)) / (2.0*KERNEL_VARIANCE));
			weightSum += weight;
			brightnessSum += thresholdedValue * weight;
		}
	}
	
	vec3 prebloomColor = texture2DRect(finalBuffer, gl_FragCoord.xy).xyz;
	vec3 brightness = vec3(brightnessSum / weightSum); 
	
	fragColor = vec4(prebloomColor + brightness, 1.0);
}