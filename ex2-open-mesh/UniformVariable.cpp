#include "UniformVariable.h"

UniformVariable::UniformVariable()
{
}

UniformVariable::~UniformVariable()
{
}

void UniformVariable::init(GLuint program, const char *name)
{
	_uv = glGetUniformLocation(program, name);
}
