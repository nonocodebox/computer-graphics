//
//  Model.h
//  cg-projects

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "bimage.h"
#include "FrameBuffer.h"
#include "PhongShader.h"
#include "GouraudShader.h"
#include "RGBShader.h"
#include "GaussianShader.h"
#include "BallShader.h"
#include "GlowShader.h"
#include "Texture.h"

class Model {
public:
	enum ShadingMode
	{
		SHADE_PHONG = 1,
		SHADE_GOURAUD = 2,
		SHADE_RGB = 3
	};
	enum TextureMode
	{
		NO_TEXTURE = 0,
		MARBLE = 1,
		WOOD = 2,
		MIRROR = 3,
		BUMP = 4,
		MODE_COUNT = 5
	};
	enum GlowMode
	{
		NO_GLOW = 0,
		GLOW_ONLY = 1,
		GLOW_BLENDED = 2,
		GLOW_MODE_COUNT = 3
	};

private:
	typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

	GLuint _vao, _vbo, _vaoBall, _vboBall, _vaoGlow, _vaoGaussian, _vboPlane;
	Texture _envMap, _brickTextureMap, _brickBumpMap;
	FrameBuffer _fbModel, _fbGlow, _fbGaussian, _fbScreen;
	
	// Shaders
	GouraudShader _shaderGouraud;
	PhongShader _shaderPhong;
	RGBShader _shaderRGB;
	GaussianShader _shaderGaussian;
	BallShader _shaderBall;
	GlowShader _shaderGlow;

	// View port frame:
	float _width, _height, _offsetX, _offsetY, _numOfModelVerices;
	float _normWidth, _normHeight;

	Mesh _mesh;
	std::vector<float> _vertices, _verticesCircle, _verticesPlane;

	glm::vec4 _lowerLeftBBox, _upperRightBBox;
	glm::mat4 _figTransform;
	glm::mat4 _direction;
	glm::vec3 _center;
	glm::mat4 _proj, _scale, _camera;

	float _viewAngle, _movementViewAngle;

	// User choice parameters
	bool _isWireframe;
	bool _isPerspective;
    bool _isFaceNormal;
    float _alpha;
    ShadingMode _mode;
	int _modelVertexCoords;

public:
	Model();
	virtual ~Model();

public:
	void init(const char* filename);
	void draw();
	void resize(int width, int height);
	void updateFigurePos(int startX, int startY, int x, int y, bool update);
	void translate(float sx, float sy, float x, float y, bool update);
	void toggleWireframeMode();
	void zoom(float sx, float sy, float x, float y, bool update);
	void toggleperspectiveMode();
	void toggleFaceNormal();
	void reset();
    void setShadingMode(ShadingMode mode);
    void scaleAlpha(bool increase);
	void changeTextureMode();
	void scaleTurbulence(bool increase);
	void scaleTexture(bool increase);
	void changeGlowMode();
	void changeKernelSize(bool increase);

private:
	glm::vec2 normalizeScreenCoords(float x, float y);
	std::vector<float> createCircleVertices();
	void loadMesh(const char* filename);
	void extractModelVertices();
	glm::vec3 map(float x, float y);
	glm::mat4 createArcballMovment(int startX, int startY, int x, int y);
	void computeBoundingBox();
	void updateProjection();
	void updateModelBuffer();
	void createBallBuffer();
	void createRGBShader();
	void createGouraudShader();
	void createPhongShader();
	void createBallShader();
	void createGlowShader();
	void createGaussianShader();
	void createPlaneBuffer();
};

#endif /* defined(__ex0__Model__) */
