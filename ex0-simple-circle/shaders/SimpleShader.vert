#version 330

layout(location = 0) in vec4 position;
uniform vec2 screenSize;

void main()
{
	if (screenSize.x < screenSize.y)
	{
		gl_Position.x = position.x;
		gl_Position.y = position.y * (screenSize.x / screenSize.y);
	}
	else
	{
		gl_Position.y = position.y;
		gl_Position.x = position.x * (screenSize.y / screenSize.x);
	}
	gl_Position.z = position.z;
	gl_Position.w = position.w;
}
