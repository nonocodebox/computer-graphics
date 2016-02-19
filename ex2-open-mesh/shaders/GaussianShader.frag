#version 330

in vec2 texturePosition;
in vec4 gl_FragCoord;
out vec4 outColor;
uniform sampler2D inputMap;
uniform int direction;
uniform float kernelSize;

float sigma2_factor = 0.6;
float PI = 3.1415926535897;

void main()
{
	vec2 inputSize = textureSize(inputMap, 0);
	float inputLen;

	float sigma2 = kernelSize * sigma2_factor;
	float A = 1.0 / sqrt(2 * PI * sigma2);
	
	vec3 glow = vec3(0);
	vec2 dirVec;

	if (direction == 0)
	{
		dirVec = vec2(1, 0);
		inputLen = inputSize.x;
	}
	else
	{
		dirVec = vec2(0, 1);
		inputLen = inputSize.y;
	}

	for (float x = 0; x < kernelSize; x++)
	{
		vec3 pixel = texture(inputMap, texturePosition + dirVec * ((x - kernelSize / 2) / inputLen)).xyz;

		if (direction == 0)
		{
			pixel = vec3((pixel.x + pixel.y + pixel.z) / 3); // Convert to gray
			pixel = (pixel.xyz - vec3(1)) * 2 + vec3(1); // Attenutate dark colors
		}

		pixel = max(min(pixel, 1), 0);

		float g = A * exp(-pow(x - kernelSize / 2, 2) / (2 * sigma2));

		glow += g * pixel;
	}
	
	outColor = vec4(glow, 1);
}
