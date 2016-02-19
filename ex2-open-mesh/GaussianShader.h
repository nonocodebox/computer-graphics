#ifndef GAUSSIAN_SHADER_H
#define GAUSSIAN_SHADER_H

#include "Shader.h"
#include "UniformVariable.h"

class GaussianShader :
	public Shader
{
public:
	GaussianShader();
	~GaussianShader();

	void setupAttributes();
	int getVertexCoords() { return 2; }

	TextureUniformVariable inputMap;
	IntUniformVariable direction;
	FloatUniformVariable kernelSize;

protected:
	void onLoad();
	void onUse();
};

#endif
