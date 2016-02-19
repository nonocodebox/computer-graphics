#include "Shader.h"
#include "ShaderIO.h"

Shader::Shader()
	: _program(0)
{
}

Shader::~Shader()
{
}

void Shader::load(const char *name, const char *vert, const char *frag)
{
	programManager::sharedInstance().createProgram(name, vert, frag);

	_program = programManager::sharedInstance().programWithID(name);

	onLoad();
}

void Shader::use()
{
	glUseProgram(_program);

	onUse();
}
