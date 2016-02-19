#version 330

layout(location = 0) in vec4 position;
uniform vec2 screenSize;
uniform mat4 transform;

void main()
{
	gl_Position = transform * position;
	//gl_Position = position;
	if (screenSize.x < screenSize.y)
	{
		gl_Position.x = gl_Position.x;
		gl_Position.y = gl_Position.y * (screenSize.x / screenSize.y);
	}
	else
	{
		gl_Position.y = gl_Position.y;
		gl_Position.x = gl_Position.x * (screenSize.y / screenSize.x);
	}
	gl_Position.z = gl_Position.z;
	gl_Position.w = gl_Position.w;
}
