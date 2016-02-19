#version 330

layout(location = 0) in vec4 position;
uniform mat4 transform;
uniform vec3 dimensions;
uniform vec3 mins;
out vec4 color;

void main()
{
	// Calculate the color relative to the vertex's position in the bounding box
	color.x = (position.x - mins.x) / dimensions.x;
	color.y = (position.y - mins.y) / dimensions.y;
	color.z = (position.z - mins.z) / dimensions.z;

	// Transform the vertex
	gl_Position = transform * position;
}
