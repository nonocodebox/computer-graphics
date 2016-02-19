#ifndef BALL_SHADER_H
#define BALL_SHADER_H

#include "Shader.h"
#include "UniformVariable.h"

class BallShader :
	public Shader
{
public:
	BallShader();
	~BallShader();

	void setupAttributes();
	int getVertexCoords() { return 4; }

	Mat4UniformVariable transform;

protected:
	void onLoad();
	void onUse();
};

#endif


