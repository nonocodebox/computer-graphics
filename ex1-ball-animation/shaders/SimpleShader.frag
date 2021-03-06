#version 330

uniform vec2 screenSize;
uniform vec4 fillColor;
uniform vec2 lightSource1;
uniform vec2 lightSource2;
uniform vec2 ballCenter;
uniform float ballRadius;

in vec4 gl_FragCoord;

out vec4 outColor;

vec4 getLightColor(vec2 fragCoord, vec2 lightSource)
{
	vec2 lightDir = normalize(lightSource - ballCenter);

	// Create a point that will be the center of light on the ball (in ball coordinates)
	vec2 lightCenter = lightDir * ballRadius * 0.5;

	// Create a white color vector
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	
	// Calculate the distance between the fragment and the light center
	float dist = length(fragCoord - lightCenter);

	// Calculate the ratio (1 = fragment is on light center)
	float ratio = 1 - dist / ballRadius;
	
	// Calculate the lighting generated by the light for the current fragment
	vec4 lightColor = (1 - ratio) * fillColor + ratio * white;

	return lightColor;
}

void main()
{
	// Translate the screen origin to the screen center and scale
	// to get world coordinates
	vec2 fragCoord = vec2(
		(gl_FragCoord.x) / (screenSize.x / 2) - 1,
		(gl_FragCoord.y) / (screenSize.y / 2) - 1
	);

	// Adjust to original coordinates (reverse vertex shader operation)
	if (screenSize.x < screenSize.y)
	{
		fragCoord.y = fragCoord.y / (screenSize.x / screenSize.y);
	}
	else
	{
		fragCoord.x = fragCoord.x / (screenSize.y / screenSize.x);
	}

	// Translate to ball coordinates
	fragCoord -= ballCenter;

	// Calculate the light colors
	vec4 lightColor1 = getLightColor(fragCoord, lightSource1);
	vec4 lightColor2 = getLightColor(fragCoord, lightSource2);
	
	// Add some ambient light to make things look better.
	// The color might be clipped at 1.0 (white) if both lights are shining on the same pixel.
	outColor = 0.2 * fillColor + 0.5 * lightColor1 + 0.5 * lightColor2;

	// Calculate the fragment's radius (distance from the ball center)
	float fragRadius = length(fragCoord);

	// Generate a smooth step function for the alpha channel (going from 1 to 0)
	float alpha = 1 - smoothstep(ballRadius - fwidth(fragRadius), ballRadius, fragRadius);

	// Set the alpha
	outColor.w = alpha;
}
