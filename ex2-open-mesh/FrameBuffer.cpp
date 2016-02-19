#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
	: _width(0), _height(0), _texture(0), _frameBuffer(0), _depthBuffer(0)
{
}

FrameBuffer::~FrameBuffer()
{
	if (_frameBuffer != 0)
	{
		glDeleteFramebuffers(1, &_frameBuffer);
	}

	if (_texture != 0)
	{
		glDeleteTextures(1, &_texture);
	}

	if (_depthBuffer != 0)
	{
		glDeleteRenderbuffers(1, &_depthBuffer);
	}
}

void FrameBuffer::createTexture()
{
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

	// The texture we're going to render to
	glGenTextures(1, &_texture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _texture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

	// Set _texture as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);

	// Set the list of draw buffers.
	GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, buffers);
}

void FrameBuffer::bind()
{
	bind(true);
}

void FrameBuffer::bind(bool clear)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);

	if (clear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FrameBuffer::resize(GLsizei width, GLsizei height)
{
	_width = width;
	_height = height;

	if (_frameBuffer != 0)
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	}
}
