#ifndef RGB_SHADER_H
#define RGB_SHADER_H

#include "Shader.h"
#include "UniformVariable.h"

class RGBShader :
	public Shader
{
public:
	RGBShader();
	~RGBShader();

	void setupAttributes();
	int getVertexCoords() { return 4; }

	Mat4UniformVariable transform;
	Vec3UniformVariable dimensions;
	Vec3UniformVariable minBoundery;

protected:
	void onLoad();
	void onUse();
};

#endif
