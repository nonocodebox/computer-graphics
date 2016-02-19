#include "BallShader.h"

BallShader::BallShader()
{
}

BallShader::~BallShader()
{
}

void BallShader::setupAttributes()
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

void BallShader::onLoad()
{
	transform.init(_program, "transform");
}

void BallShader::onUse()
{
	transform.transfer();
}
