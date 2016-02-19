#include "GaussianShader.h"

GaussianShader::GaussianShader()
{
}

GaussianShader::~GaussianShader()
{
}

void GaussianShader::setupAttributes()
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

void GaussianShader::onLoad()
{
	inputMap.init(_program, "inputMap");
	direction.init(_program, "direction");
	kernelSize.init(_program, "kernelSize");
	inputMap.index = 0;
}

void GaussianShader::onUse()
{
	inputMap.transfer();
	direction.transfer();
	kernelSize.transfer();
}
