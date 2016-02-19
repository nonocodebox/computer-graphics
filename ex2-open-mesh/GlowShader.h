#ifndef GLOW_SHADER_H
#define GLOW_SHADER_H

#include "Shader.h"
#include "UniformVariable.h"

class GlowShader :
	public Shader
{
public:
	GlowShader();
	~GlowShader();

	void setupAttributes();
	int getVertexCoords() { return 2; }

	TextureUniformVariable frameMap;
	TextureUniformVariable glowMap;
	IntUniformVariable glowMode;

protected:
	void onLoad();
	void onUse();
};

#endif
