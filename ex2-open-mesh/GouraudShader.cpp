#include "GouraudShader.h"

GouraudShader::GouraudShader()
{
}

GouraudShader::~GouraudShader()
{
}

void GouraudShader::setupAttributes()
{
	GLint posAttrib = glGetAttribLocation(_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, // attribute handle
		4,          // number of scalars per vertex
		GL_FLOAT,   // scalar type
		GL_FALSE,
		7 * sizeof(GLfloat),
		0);
	GLint normalAttrib = glGetAttribLocation(_program, "normal");
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, // attribute handle
		3,          // number of scalars per vertex
		GL_FLOAT,   // scalar type
		GL_FALSE,
		7 * sizeof(GLfloat),
		(const GLvoid *)(4 * sizeof(GLfloat)));
}

void GouraudShader::onLoad()
{
	transform.init(_program, "projTransform");
	transformView.init(_program, "viewTransform");
	shineCoeff.init(_program, "alpha");
}

void GouraudShader::onUse()
{
	transform.transfer();
	transformView.transfer();
	shineCoeff.transfer();
}
