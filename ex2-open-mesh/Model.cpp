//
//  Model.cpp
//  cg-projects
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

//#include <OpenMesh/Tools/Kernel_OSG/ArrayKernelT.hh>

#include <math.h>
#include <algorithm>

#include "bimage.h"

#define PI						3.1415926535897f
#define THRESHOLD_SIZE			400
#define WINDOW_SIZE				600
#define ARCBALL_RADIUS			0.85f
#define ALPHA					10
#define OBJECT_DEPTH			5.5f // Choose R/sin(FOV/2) so that the entire ball will fit the view frustum
#define OBJECT_B_RAD			3.8f
#define OBJECT_RADIUS			1.45f // Choose a radius which contains the box (-1..1, -1..1, -1..1) - Radius is sqrt(2)
#define Z_NEAR					(OBJECT_DEPTH - OBJECT_B_RAD)
#define Z_FAR					(OBJECT_DEPTH + OBJECT_B_RAD)
#define INITIAL_FOV				30.0f
#define MAX_FOV					150.0f
#define MIN_FOV					10.0f
#define ALPHA_FACTOR			10.0f
#define INITIAL_ALPHA			200.0f
#define TURB_FACTOR				0.1f
#define TEXTURE_FACTOR			0.1f
#define INITIAL_TURB_WOOD		3.0f
#define INITIAL_TEXTURE_WOOD	2.5f
#define INITIAL_TURB_MARBLE		1.3f
#define INITIAL_TEXTURE_MARBLE	3.1f
#define INITIAL_KERNEL_SIZE		21.0f

#define SHADERS_DIR "shaders/"


Model::Model() :
_vao(0), _vbo(0), _vaoBall(0), _vboBall(0),_vaoGlow(0), _vaoGaussian(0), _vboPlane(0), 
_width(WINDOW_SIZE), _height(WINDOW_SIZE), _normWidth(1.0f), _normHeight(1.0f),
_figTransform(1.0f), _direction(1.0f), _viewAngle(INITIAL_FOV), _movementViewAngle(0),
_isWireframe(false), _isPerspective(true), _isFaceNormal(false), _alpha(INITIAL_ALPHA), _mode(SHADE_PHONG), _modelVertexCoords(4)
{
	_shaderPhong.state = MARBLE;
	_shaderPhong.turbulenceCoeff = INITIAL_TURB_MARBLE;
	_shaderPhong.textureScale = INITIAL_TEXTURE_MARBLE;
	_shaderGlow.glowMode = NO_GLOW;
	_shaderGaussian.kernelSize = INITIAL_KERNEL_SIZE;
}

Model::~Model()
{
    _mesh.release_vertex_normals();
	_mesh.release_face_normals();

	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_vaoBall != 0)
		glDeleteVertexArrays(1, &_vaoBall);
	if (_vboBall != 0)
		glDeleteBuffers(1, &_vboBall);
	if (_vaoGlow != 0)
		glDeleteVertexArrays(1, &_vaoGlow);
	if (_vaoGaussian != 0)
		glDeleteVertexArrays(1, &_vaoGaussian);
	if (_vboPlane != 0)
		glDeleteBuffers(1, &_vboPlane);
}

//Load a mesh from the given input file
void Model::loadMesh(const char* filename)
{
	if (!OpenMesh::IO::read_mesh(_mesh, filename))
	{
		// if we didn't make it, exit...
		fprintf(stderr, "Error loading mesh, Aborting.\n");
		exit(-1);
	}

	_mesh.request_vertex_normals();
	_mesh.request_face_normals();
	_mesh.update_normals();
}

void Model::extractModelVertices()
{
	Mesh::FaceHandle fHandle;

	_vertices.clear();

	/* go over all the faces */
	for (Mesh::FaceIter fIter = _mesh.faces_begin(); fIter != _mesh.faces_end(); ++fIter)
	{
		/* for each face - go over all the vertices that belong to it */
		for (Mesh::FaceVertexIter fvIter = _mesh.fv_iter(fIter); fvIter; ++fvIter)
		{
			Mesh::Point p = _mesh.point(fvIter);
			for (int i = 0; i < 3; i++)
			{
				_vertices.push_back(p[i]);
			}
			_vertices.push_back(1.0f);

            if (_mode != SHADE_RGB)
            {
				Mesh::Normal n = _isFaceNormal ? _mesh.normal(fIter) : _mesh.normal(fvIter);
				n.normalize();

                for (int i = 0; i < 3; i++)
    			{
    				_vertices.push_back(n[i]);
    			}
            }
		}
	}
}

/** This function computes the geometrical center and the axis aligned bounding box of the
object. The bounding box is represented by the lower left and upper right corners. */
void Model::computeBoundingBox()
{
	// Vertex iterator is an iterator which goes over all the vertices of the _mesh:
	Mesh::VertexIter vertexIter;
	// This is used to store the geometrical position of the vertices of the _mesh:
	Mesh::Point p;
	const float fm = std::numeric_limits<float>::max();
	Mesh::Point lowerLeft(fm, fm, fm); Mesh::Point upperRight(0, 0, 0);
	// number of vertices in the _mesh:
	vertexIter = _mesh.vertices_begin();
	lowerLeft = upperRight = _mesh.point(*vertexIter);
	// This is how to go over all the vertices in the _mesh:
	for (++vertexIter; vertexIter != _mesh.vertices_end(); ++vertexIter){
		// This is how to get the point associated with the vertex:
		p = _mesh.point(*vertexIter);
		for (int i = 0; i < 3; i++) {
			lowerLeft[i] = std::min(lowerLeft[i], p[i]);
			upperRight[i] = std::max(upperRight[i], p[i]);
		}
	}

	// Compute the coreners and center of the bounding box
	_lowerLeftBBox = glm::vec4(lowerLeft[0], lowerLeft[1], lowerLeft[2], 1.0f);
	_upperRightBBox = glm::vec4(upperRight[0], upperRight[1], upperRight[2], 1.0f);
	_center = glm::vec3(lowerLeft[0] + (upperRight[0] - lowerLeft[0]) / 2,
									    lowerLeft[1] + (upperRight[1] - lowerLeft[1]) / 2,
										lowerLeft[2] + (upperRight[2] - lowerLeft[2]) / 2);
}

glm::mat4 Model::createArcballMovment(int startX, int startY, int x, int y)
{
	// Map the start and destination points to the arcball
	glm::vec3 start = map((float)x, (float)y);
	glm::vec3 dest = map((float)startX, (float)startY);

	// Verify the movement is not too small
	if (glm::length(start - dest) < 0.001)
	{
		return glm::mat4(1.0f);
	}

	// Calculate the movement and create a rotation matrix
	glm::vec3 u = glm::normalize(glm::cross(start, dest));

	float theta = glm::acos(glm::dot(glm::normalize(start), glm::normalize(dest)));
	theta = theta * 2;

	float c = std::cos(theta);
	float oc = 1 - c;
	float s = std::sin(theta);
	// https://en.wikipedia.org/wiki/Rotation_matrix
	glm::mat4 rotArbitrary = glm::mat4(
		c + u[0] * u[0] * oc, u[0] * u[1] * oc - u[2] * s, u[0] * u[2] * oc + u[1] * s, 0,
		u[1] * u[0] * oc + u[2] * s, c + u[1] * u[1] * oc, u[1] * u[2] * oc - u[0] * s, 0,
		u[2] * u[0] * oc - u[1] * s, u[2] * u[1] * oc + u[0] * s, c + u[2] * u[2] * oc, 0,
		0, 0, 0, 1);

	return rotArbitrary;
}

glm::vec2 Model::normalizeScreenCoords(float x, float y)
{
	// Move the origin to the center and flip Y
	x -= _width / 2;
	y -= _height / 2;
	y = -y;

	// Normalize
	x /= (_width / 2);
	y /= (_height / 2);

	x *= _normWidth; // Adjust to the screen aspect ratio
	y *= _normHeight;

	return glm::vec2(x, y);
}

glm::vec3 Model::map(float x, float y)
{
	glm::vec2 v = normalizeScreenCoords(x, y); // Normalize to screen coordinates
	glm::vec3 map_vec = glm::vec3(v.x, v.y, 0);
	float radius = glm::length(v);

	if (radius <= ARCBALL_RADIUS)
	{
		// IF inside the arcball, calculate Z
		map_vec[2] = (float)std::sqrt(std::pow(ARCBALL_RADIUS, 2) - std::pow(v.x, 2) - std::pow(v.y, 2));
	}

	return map_vec;
}

std::vector<float> Model::createCircleVertices()
{
	std::vector<float> vertices;
	int theta = 0;

	// Add vertices on the perimeter of the circle until we've
	// covered a whole circle
	while (theta <= 360)
	{
		vertices.push_back(ARCBALL_RADIUS * cos(theta * (PI / 180)));
		vertices.push_back(ARCBALL_RADIUS * sin(theta * (PI / 180)));
		vertices.push_back(0.0f);
		vertices.push_back(1.0f);
		theta += ALPHA;
	}

	return vertices;
}

void Model::updateFigurePos(int startX, int startY, int x, int y, bool update)
{
	// Update the arcball movement
	_direction = createArcballMovment(startX, startY, x, y);

	if (update)
	{
		// Update the total figure transform and reset the direction matrix
		_figTransform = _direction * _figTransform;
		_direction = glm::mat4(1.0f);
	}
}

void  Model::translate(float sx, float sy, float x, float y, bool update)
{
	glm::vec2 start = normalizeScreenCoords(sx, sy);
	glm::vec2 end = normalizeScreenCoords(x, y);

	// Get the relative movement
	glm::vec2 v = end - start;

	float translateScale = OBJECT_RADIUS; // Scale the movement to the ball radius (change from -1..1 to -1.45..1.45)
	translateScale /= INITIAL_FOV / _viewAngle; // Scale the movement according to the current zoom

	_direction = glm::translate(glm::mat4(1.0f), translateScale * glm::vec3(v.x, v.y, 0));

	if (update)
	{
		// Update the total figure transform and reset the direction matrix
		_figTransform = _direction * _figTransform;
		_direction = glm::mat4(1.0f);
	}
}

void Model::updateModelBuffer()
{
	extractModelVertices();

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);

	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	// Obtain attribute handles:

	glBindVertexArray(_vao);

	Shader *shader;

	if (_mode == SHADE_PHONG)
	{
		shader = &_shaderPhong;
	}
	else if (_mode == SHADE_GOURAUD)
	{
		shader = &_shaderGouraud;
	}
	else
	{
		shader = &_shaderRGB;
	}

	_modelVertexCoords = shader->getVertexCoords();
	shader->setupAttributes();

	// Unbind vertex array:
	glBindVertexArray(0);
}

void Model::createBallBuffer()
{
	_verticesCircle = createCircleVertices();

	glBindBuffer(GL_ARRAY_BUFFER, _vboBall);
	glBufferData(GL_ARRAY_BUFFER, _verticesCircle.size() * sizeof(float), &_verticesCircle[0], GL_STATIC_DRAW);

	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	// Obtain attribute handles:

	glBindVertexArray(_vaoBall);

	_shaderBall.setupAttributes();

	// Unbind vertex array:
	glBindVertexArray(0);
}

void Model::createPlaneBuffer()
{
	glGenVertexArrays(1, &_vaoGlow);
	glGenVertexArrays(1, &_vaoGaussian);

	// Create a Vertex Buffer Object:
	glGenBuffers(1, &_vboPlane);

	_verticesPlane.clear();
	_verticesPlane.push_back(-1);
	_verticesPlane.push_back(-1);

	_verticesPlane.push_back(-1);
	_verticesPlane.push_back(1);

	_verticesPlane.push_back(1);
	_verticesPlane.push_back(-1);

	_verticesPlane.push_back(1);
	_verticesPlane.push_back(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vboPlane);
	glBufferData(GL_ARRAY_BUFFER, _verticesPlane.size() * sizeof(float), &_verticesPlane[0], GL_STATIC_DRAW);

	glBindVertexArray(_vaoGlow);
	_shaderGlow.setupAttributes();

	glBindVertexArray(_vaoGaussian);
	_shaderGlow.setupAttributes();

	// Unbind vertex array:
	glBindVertexArray(0);
}

void Model::createGlowShader()
{
	_shaderGlow.load(
		"glow",
		SHADERS_DIR "GlowShader.vert",
		SHADERS_DIR "GlowShader.frag");
}

void Model::createGaussianShader()
{
	_shaderGaussian.load(
		"gaussian",
		SHADERS_DIR "GaussianShader.vert",
		SHADERS_DIR "GaussianShader.frag");
}

void Model::createRGBShader()
{
	_shaderRGB.load(
		"box",
		SHADERS_DIR "BoxShader.vert",
		SHADERS_DIR "BoxShader.frag");
}

void Model::createGouraudShader()
{
	_shaderGouraud.load(
		"gouraud",
		SHADERS_DIR "GouraudShader.vert",
		SHADERS_DIR "GouraudShader.frag");
}

void Model::createPhongShader()
{
	_shaderPhong.load(
		"phong",
		SHADERS_DIR "PhongShader.vert",
		SHADERS_DIR "PhongShader.frag");
}

void Model::createBallShader()
{
	_shaderBall.load(
		"ball",
		SHADERS_DIR "BallShader.vert",
		SHADERS_DIR "BallShader.frag");
}

void Model::init(const char* filename)
{
	createBallShader();
	createRGBShader();
	createGouraudShader();
	createPhongShader();
	createGlowShader();
	createGaussianShader();

	glEnable(GL_DEPTH_TEST);

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// Create the object's Vertex Array Object:
		glGenVertexArrays(1, &_vaoBall);
		glGenBuffers(1, &_vboBall);

		// Create a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);

		// Create the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);

		loadMesh(filename);
		createBallBuffer();
		createPlaneBuffer();
		updateModelBuffer();
		computeBoundingBox();

		float w = _upperRightBBox[0] - _lowerLeftBBox[0];
		float h = _upperRightBBox[1] - _lowerLeftBBox[1];
		float z = _upperRightBBox[2] - _lowerLeftBBox[2];
		float maxScale = std::max(std::max(w, h), z);

		glm::mat4 center = glm::translate(glm::mat4(1.0f), -_center);
		glm::mat4 camera = glm::lookAt(glm::vec3(0, 0, OBJECT_DEPTH), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
		glm::mat4 proj = glm::perspective(_viewAngle*(PI / 180), _width / _height, 1.0f, 10.0f);
		glm::mat4 scaleFitScreen = glm::scale(glm::mat4(1.0f), glm::vec3(1 / (maxScale / OBJECT_RADIUS)));
		_camera = camera;
		_proj = proj;
		_scale = scaleFitScreen * center;
		_figTransform = glm::mat4(1.0);
		reset();
	}

	_envMap.load("textures/spheremap2.bmp");
	_brickTextureMap.load("textures/brickwork-texture.bmp");
	_brickBumpMap.load("textures/brickwork-bump-map.bmp");

	_shaderPhong.envMap = _envMap.getTexture();
	_shaderPhong.brickTextureMap = _brickTextureMap.getTexture();
	_shaderPhong.brickBumpMap = _brickBumpMap.getTexture();

	_fbModel.resize((GLsizei)_width, (GLsizei)_height);
	_fbGlow.resize((GLsizei)_width, (GLsizei)_height);
	_fbGaussian.resize((GLsizei)_width, (GLsizei)_height);
	_fbScreen.resize((GLsizei)_width, (GLsizei)_height);
	_fbModel.createTexture();
	_fbGlow.createTexture();
	_fbGaussian.createTexture();
}

void Model::draw()
{
    GLenum polygonMode = _isWireframe ? GL_LINE : GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Get the total model transform and pass it to the shader
	glm::mat4 viewTransform = _camera * _direction * _figTransform * _scale;
	glm::mat4 projTransform = _proj * viewTransform;

    if(_mode == SHADE_PHONG)
	{
		_shaderPhong.shineCoeff = _alpha;
		_shaderPhong.transform = projTransform;
		_shaderPhong.transformView = viewTransform;
		_shaderPhong.dimensions = glm::vec3(_upperRightBBox - _lowerLeftBBox);
		_shaderPhong.minBoundery = glm::vec3(_lowerLeftBBox);
		_shaderPhong.use();
	}
	else if (_mode == SHADE_GOURAUD)
    {
		_shaderGouraud.shineCoeff = _alpha;
		_shaderGouraud.transform = projTransform;
		_shaderGouraud.transformView = viewTransform;
		_shaderGouraud.use();
    }
	else
	{
		_shaderRGB.transform = projTransform;
		_shaderRGB.dimensions = glm::vec3(_upperRightBBox - _lowerLeftBBox);
		_shaderRGB.minBoundery = glm::vec3(_lowerLeftBBox);
		_shaderRGB.use();
	}

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	
	if (_shaderGlow.glowMode.value == NO_GLOW)
	{
		_fbScreen.bind();
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / _modelVertexCoords);
	}
	else
	{
		_fbModel.bind();
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / _modelVertexCoords);

		/* Gaussian X blur + color attenutation */

		glBindVertexArray(_vaoGaussian);

		_fbGaussian.bind();
		_shaderGaussian.direction = 0;
		_shaderGaussian.inputMap = _fbModel.getTexture();
		_shaderGaussian.use();

		size_t numberOfGlowVertices = 4;
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numberOfGlowVertices);

		/* Gaussian Y blur */

		_fbGlow.bind();
		_shaderGaussian.inputMap = _fbGaussian.getTexture();
		_shaderGaussian.direction = 1;
		_shaderGaussian.use();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, numberOfGlowVertices);

		/* Glow blending*/

		glBindVertexArray(_vaoGlow);

		_fbScreen.bind();
		_shaderGlow.frameMap = _fbModel.getTexture();
		_shaderGlow.glowMap = _fbGlow.getTexture();
		_shaderGlow.use();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, numberOfGlowVertices);
	}

	/* Arcball */

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw the ball/circle
	// Create a transformation matrix for the ball (to fix screen's aspect ratio)
	_shaderBall.transform = glm::scale(glm::mat4(1.0), glm::vec3(1 / _normWidth, 1 / _normHeight, 1.0f));
	_shaderBall.use();

	glBindVertexArray(_vaoBall);

	// Draw the ball/circle
	size_t numberOfCircleVertices = _verticesCircle.size() / _shaderBall.getVertexCoords();
	glDrawArrays(GL_LINE_STRIP, 0, numberOfCircleVertices);

	// Unbind the Vertex Array object
	glBindVertexArray(0);

	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::toggleWireframeMode()
{
	_isWireframe = !_isWireframe;

	if (_isWireframe)
	{
		_shaderGlow.glowMode.value = NO_GLOW;
	}
}

void Model::toggleperspectiveMode()
{
	_isPerspective = !_isPerspective;

	updateProjection();
}

void Model::updateProjection()
{
	float angle = _viewAngle + _movementViewAngle;

	if (_isPerspective)
	{
		// Calculate perspective projection
		_proj = glm::perspective(angle * (PI / 180), _width / _height, Z_NEAR, Z_FAR);
	}
	else
	{
		// Calculate orthographic projection and then zoom by scaling
		_proj =
			glm::scale(glm::mat4(1.0f), glm::vec3(INITIAL_FOV / angle)) * // Zoom
			glm::ortho(
				-OBJECT_RADIUS * _normWidth, OBJECT_RADIUS * _normWidth, // Adjust to the screen's aspect ratio
				-OBJECT_RADIUS * _normHeight, OBJECT_RADIUS * _normHeight,
				Z_NEAR, Z_FAR);
	}
}

void Model::toggleFaceNormal()
{
    _isFaceNormal = !_isFaceNormal;
	_mode = SHADE_GOURAUD;

	updateModelBuffer();
}

void Model::setShadingMode(ShadingMode mode)
{
	if (mode == _mode)
	{
		return;
	}

    _mode = mode;

	if (mode == SHADE_PHONG)
	{
		// Phong requires vertex normals
		_isFaceNormal = false;
	}

	updateModelBuffer();
}

void Model::scaleAlpha(bool increase)
{
    _alpha = increase ? _alpha + ALPHA_FACTOR : _alpha - ALPHA_FACTOR;
    _alpha = (_alpha <= 0.0) ? 0 : _alpha;
    _alpha = (_alpha > 2000.0f) ? 2000.0f : _alpha;
}

void Model::changeKernelSize(bool increase)
{
	_shaderGaussian.kernelSize.value += increase ? 1 : -1;
	_shaderGaussian.kernelSize = (_shaderGaussian.kernelSize.value <= 0) ? 0 : _shaderGaussian.kernelSize.value;
	_shaderGaussian.kernelSize = (_shaderGaussian.kernelSize.value > 50) ? 50 : _shaderGaussian.kernelSize.value;
}

void Model::changeTextureMode()
{
	if (_mode != SHADE_PHONG)
	{
		return;
	}
	
	_shaderPhong.state = (TextureMode)((_shaderPhong.state.value + 1) % MODE_COUNT);

	if (_shaderPhong.state.value == WOOD)
	{
		_shaderPhong.turbulenceCoeff = INITIAL_TURB_WOOD;
		_shaderPhong.textureScale = INITIAL_TEXTURE_WOOD;
	}
	else if (_shaderPhong.state.value == MARBLE)
	{
		_shaderPhong.turbulenceCoeff = INITIAL_TURB_MARBLE;
		_shaderPhong.textureScale = INITIAL_TEXTURE_MARBLE;
	}
}
void Model::scaleTurbulence(bool increase)
{
	_shaderPhong.turbulenceCoeff.value += increase ? TURB_FACTOR : -TURB_FACTOR;
	_shaderPhong.turbulenceCoeff = (_shaderPhong.turbulenceCoeff.value <= 0.0) ? 0 : _shaderPhong.turbulenceCoeff.value;
}
void Model::scaleTexture(bool increase)
{
	_shaderPhong.textureScale.value += increase ? TEXTURE_FACTOR : -TEXTURE_FACTOR;
	_shaderPhong.textureScale = (_shaderPhong.textureScale.value <= 0.0) ? 0 : _shaderPhong.textureScale.value;
}

void Model::changeGlowMode()
{
	_shaderGlow.glowMode = (GlowMode)((_shaderGlow.glowMode.value + 1) % GLOW_MODE_COUNT);

	if (_shaderGlow.glowMode.value != NO_GLOW)
	{
		_isWireframe = false;
	}
}

void Model::reset()
{
	// Reset the transformation matrices and viewing angle
	_figTransform = glm::mat4(1.0f);
	_direction = glm::mat4(1.0f);
	_viewAngle = INITIAL_FOV;
	_alpha = INITIAL_ALPHA;
	_shaderPhong.state = MARBLE;
	_shaderPhong.turbulenceCoeff = INITIAL_TURB_MARBLE;
	_shaderPhong.textureScale = INITIAL_TEXTURE_MARBLE;
	_shaderGlow.glowMode = NO_GLOW;
	_shaderGaussian.kernelSize = INITIAL_KERNEL_SIZE;

	updateProjection();
}

void Model::zoom(float sx, float sy, float x, float y, bool update)
{
	glm::vec2 start = normalizeScreenCoords(sx, sy);
	glm::vec2 end = normalizeScreenCoords(x, y);

	// Calculate the movement and scale to angles
	_movementViewAngle = (end.y - start.y) * 60;

	// Clip angle at [MIN_FOV, MAX_FOV]
	if (_viewAngle + _movementViewAngle < MIN_FOV)
	{
		_movementViewAngle = MIN_FOV - _viewAngle;
	}

	if (_viewAngle + _movementViewAngle > MAX_FOV)
	{
		_movementViewAngle = MAX_FOV - _viewAngle;
	}

	if (update)
	{
		// Update the total view angle and reset the movement view angle
		_viewAngle += _movementViewAngle;
		_movementViewAngle = 0;
	}

	updateProjection();
}

void Model::resize(int width, int height)
{
    _width	= (float)width;
    _height = (float)height;
    _offsetX = 0;
    _offsetY = 0;

	// Calculate the normalized screen width and height
	if (width > height)
	{
		_normWidth = _width / _height;
		_normHeight = 1.0f;
	}
	else
	{
		_normWidth = 1.0f;
		_normHeight = _height / _width;
	}

	_fbModel.resize((GLsizei)_width, (GLsizei)_height);
	_fbGlow.resize((GLsizei)_width, (GLsizei)_height);
	_fbGaussian.resize((GLsizei)_width, (GLsizei)_height);
	_fbScreen.resize((GLsizei)_width, (GLsizei)_height);

	updateProjection();
}
