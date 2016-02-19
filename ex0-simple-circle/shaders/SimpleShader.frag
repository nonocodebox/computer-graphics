#version 330

uniform vec4 fillColor1, fillColor2;

in vec4 gl_FragCoord;

out vec4 outColor;

void main()
{
	if ((mod(gl_FragCoord[1], 20) < 10 && mod(gl_FragCoord[0], 20) < 10)
      || (mod(gl_FragCoord[1], 20) >= 10 && mod(gl_FragCoord[0], 20) >= 10))
	{
		outColor = fillColor1;
	}
	else
	{
		outColor = fillColor2;
	}
}
