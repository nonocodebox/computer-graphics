#include "GlowShader.h"

GlowShader::GlowShader()
{
}

GlowShader::~GlowShader()
{
}

void GlowShader::setupAttributes()
{
	GLint posAttrib = glGetAttribLocation(_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, // attribute handle
		2,          // number of scalars per vertex
		GL_FLOAT,   // scalar type
		GL_FALSE,
		0,
		0);
}

void GlowShader::onLoad()
{
	frameMap.init(_program, "frameMap");
	glowMap.init(_program, "glowMap");
	glowMode.init(_program, "glowMode");
	frameMap.index = 0;
	glowMap.index = 1;
}

void GlowShader::onUse()
{
	frameMap.transfer();
	glowMap.transfer();
	glowMode.transfer();
}
