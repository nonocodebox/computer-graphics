#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void createTexture();
	void resize(GLsizei width, GLsizei height);
	void bind();
	void bind(bool clear);
	GLuint getTexture() { return _texture; }

private:
	GLsizei _width, _height;
	GLuint _texture, _frameBuffer, _depthBuffer;
};

