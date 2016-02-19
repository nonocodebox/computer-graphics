#version 330

layout(location = 0) in vec2 position;
out vec2 texturePosition;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	texturePosition = position * 0.5 + vec2(0.5);
}
