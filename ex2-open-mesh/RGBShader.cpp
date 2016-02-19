#include "RGBShader.h"

RGBShader::RGBShader()
{
}

RGBShader::~RGBShader()
{
}

void RGBShader::setupAttributes()
{
	GLint posAttrib = glGetAttribLocation(_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, // attribute handle
		4,          // number of scalars per vertex
		GL_FLOAT,   // scalar type
		GL_FALSE,
		0,
		0);
}

void RGBShader::onLoad()
{
	transform.init(_program, "transform");
	dimensions.init(_program, "dimensions");
	minBoundery.init(_program, "mins");
}

void RGBShader::onUse()
{
	transform.transfer();
	dimensions.transfer();
	minBoundery.transfer();
}
