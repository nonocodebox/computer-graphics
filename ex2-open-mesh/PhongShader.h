#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H
#include "Shader.h"
#include "UniformVariable.h"

class PhongShader :
	public Shader
{
public:
	PhongShader();
	~PhongShader();

	void setupAttributes();
	int getVertexCoords() { return 7; }

	Mat4UniformVariable transform;
	Mat4UniformVariable transformView;
	Vec3UniformVariable dimensions;
	Vec3UniformVariable minBoundery;
	FloatUniformVariable shineCoeff;
	IntUniformVariable state;
	FloatUniformVariable turbulenceCoeff, textureScale;
	TextureUniformVariable envMap, brickTextureMap, brickBumpMap;

protected:
	void onLoad();
	void onUse();
};


#endif
