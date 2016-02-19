#version 330

in vec2 texturePosition;
in vec4 gl_FragCoord;
out vec4 outColor;
uniform sampler2D frameMap;
uniform sampler2D glowMap;
uniform int glowMode;

void main()
{
	vec3 glow = texture(glowMap, texturePosition).xyz;
	vec3 screen = texture(frameMap, texturePosition).xyz;

	if (glowMode == 0)
	{
		outColor = vec4(screen, 1);
	}
	else if (glowMode == 1)
	{
		outColor = vec4(glow, 1);
	}
	else
	{
		outColor = vec4(glow + screen - glow * screen, 1);
	}
}
