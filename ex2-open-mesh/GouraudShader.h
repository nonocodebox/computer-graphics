#ifndef GOURAUD_SHADER_H
#define GOURAUD_SHADER_H
#include "Shader.h"
#include "UniformVariable.h"

class GouraudShader :
	public Shader
{
public:
	GouraudShader();
	~GouraudShader();

	void setupAttributes();
	int getVertexCoords() { return 7; }

	Mat4UniformVariable transform;
	Mat4UniformVariable transformView;
	FloatUniformVariable shineCoeff;

protected:
	void onLoad();
	void onUse();
};

#endif
