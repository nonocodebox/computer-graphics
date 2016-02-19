#version 330

in vec4 gl_FragCoord;
in vec4 color;
out vec4 outColor;

void main()
{
	outColor.xyz = color.xyz;
    outColor.w = 1.0;
}
