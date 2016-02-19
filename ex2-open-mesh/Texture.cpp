#include "Texture.h"

Texture::Texture()
	: _texture(0)
{
}

Texture::~Texture()
{
	if (_texture != 0)
	{
		glDeleteTextures(1, &_texture);
	}
}

void Texture::load(const char *path)
{
	load(BImage(path));
}

void Texture::load(const BImage &image)
{
	if (_texture)
	{
		glDeleteTextures(1, &_texture);
	}

	glGenTextures(1, &_texture); // Generate a texture in OpenGL
	glBindTexture(GL_TEXTURE_2D, _texture); // Bind texture before setting its properties

	// Setting texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Transfer data to texture:
	glTexImage2D(GL_TEXTURE_2D,
		0, // level
		GL_RGBA, // Internal representation
		image.width(), image.height(), // Texture size
		0, // must be 0
		GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
		image.getImageData()); // Pixel data
	glBindTexture(GL_TEXTURE_2D, 0);
}
