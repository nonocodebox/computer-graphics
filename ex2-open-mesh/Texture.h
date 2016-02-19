#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include "bimage.h"

class Texture
{
public:
	Texture();
	~Texture();

	void load(const char *path);
	void load(const BImage &image);

	GLuint getTexture() const { return _texture; }

private:
	GLuint _texture;
};

#endif
