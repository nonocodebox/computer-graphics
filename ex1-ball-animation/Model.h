//
//  Model.h
//  cg-projects
//

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
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Model {
	class BallProperties {
	public:
		BallProperties(glm::mat4 transform, glm::vec3 direction, glm::vec4 color, float radius)
			: _transform(transform), _direction(direction), _color(color), _radius(radius), _currentRadius(radius)
		{
		}
		
		glm::mat4 _transform; // Cumulative transformation matrix
		glm::vec3 _direction; // Current direction
		glm::vec4 _color; // Color
		float _radius; // Maximal radius
		float _currentRadius; // Current radius
	};

	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	GLint _screenSizeUV;
	GLint _transformUV;
	GLint _lightSource1UV, _lightSource2UV;
	GLint _ballCenterUV;
	GLint _ballRadiusUV;
	
	// View port frame:
	float _width, _height, _offsetX, _offsetY, _edgeX, _edgeY;

	int _alpha;
	int _prev_alpha;

	typedef std::vector<BallProperties> BallTransforms;

	std::vector<float> _vertices;

	// Ball transformations and properties
	BallTransforms _ballTransform;

	// Light source positions
	glm::vec4 _lightSource1, _lightSource2;

	// Origin constant (in ball space, (0, 0, 0, 1))
	const glm::vec4 _origin;

public:
	Model();

public:
	virtual ~Model();

public:
	void init();
	
public:
	void draw();
	void step();

	// Adds a ball at screen coordinates (x, y)
	void addBall(int x, int y);
	
public:
	void resize(int width, int height);

private:
	// Creates and returns the ball vertices
	std::vector<float> createVertices();

	// Recalculates the ball vertices and updates the buffer
	// (for recreating the balls with more/less vertices due to window resize)
	void redefineVertices();
	
	// Recalculates the ball vertices
	void updateBufferVertices();

	// Check for wall collisions
	bool isWallCollision(unsigned int ball, bool yAxis, float &offset);

	// Gets the ball collision radius for a ball:
	// min(distance to nearest ball, ball radius)
	float getBallCollisionRadius(unsigned int ball);

	// Transforms the ball according to window size rescale
	void scaleByWindowSize();
};

#endif /* defined(__ex0__Model__) */
