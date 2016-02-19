#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class Shader
{
public:
	Shader();
	virtual ~Shader();

	void load(const char *name, const char *vert, const char *frag);
	void use();

	virtual void setupAttributes() = 0;
	virtual int getVertexCoords() = 0;

protected:
	GLuint _program;

	virtual void onLoad() = 0;
	virtual void onUse() = 0;
};

#endif
