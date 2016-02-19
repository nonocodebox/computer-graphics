#include "PhongShader.h"

PhongShader::PhongShader()
{
}

PhongShader::~PhongShader()
{
}

void PhongShader::setupAttributes()
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

void PhongShader::onLoad()
{
	transform.init(_program, "projTransform");
	transformView.init(_program, "viewTransform");
	shineCoeff.init(_program, "alpha");
	state.init(_program, "state");
	turbulenceCoeff.init(_program, "turbulenceCoeff");
	textureScale.init(_program, "textureScale");
	dimensions.init(_program, "dimensions");
	minBoundery.init(_program, "mins");

	envMap.init(_program, "envMap");
	brickTextureMap.init(_program, "brickTextureMap");
	brickBumpMap.init(_program, "brickBumpMap");

	envMap.index = 0;
	brickTextureMap.index = 1;
	brickBumpMap.index = 2;
}

void PhongShader::onUse()
{
	transform.transfer();
	transformView.transfer();
	shineCoeff.transfer();
	state.transfer();
	turbulenceCoeff.transfer();
	textureScale.transfer();
	dimensions.transfer();
	minBoundery.transfer();

	envMap.transfer();
	brickTextureMap.transfer();
	brickBumpMap.transfer();
}
