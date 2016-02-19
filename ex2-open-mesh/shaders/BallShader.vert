#version 330

layout(location = 0) in vec4 position;
uniform mat4 transform;

void main()
{
	// Transform the vertex (adjust for screen aspect ratio)
	gl_Position = transform * position;
}
