#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform vec3 dimensions;
uniform vec3 mins;
uniform mat4 projTransform;
uniform mat4 viewTransform;
out vec3 texturePosition;
out vec3 viewPos;
out vec3 tnormal;

void main()
{
	// Transform the vertex
	gl_Position = projTransform * position;

	viewPos = (viewTransform * position).xyz;
	tnormal = (viewTransform * vec4(normal, 0)).xyz;

	// Calculate the texture mapping to the vertex's position in the bounding box
	texturePosition.x = (position.x - mins.x) / dimensions.x;
	texturePosition.y = (position.y - mins.y) / dimensions.y;
	texturePosition.z = (position.z - mins.z) / dimensions.z;
}
