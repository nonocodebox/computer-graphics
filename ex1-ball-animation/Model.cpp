//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
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

#include <math.h>
#include <algorithm>

#define PI					3.1415926535897f
#define BIG_ALPHA_ANGLE		20
#define SMALL_ALPHA_ANGLE	30
#define THRESHOLD_SIZE		400
#define RADIUS				0.1f
#define WINDOW_SIZE			600
#define EPSILON				0.000001
#define RADIUS_TRESHHOLD	0.001
#define X_LIGHT1			0.7
#define Y_LIGHT1			1.2
#define X_LIGHT2			-2.4
#define Y_LIGHT2			0.8
#define MIN_COLOR			0.2
#define MAX_COLOR			0.6
#define BALL_SPEED			0.01f

#define SHADERS_DIR "shaders/"

Model::Model() :
_vao(0), _vbo(0), _width(WINDOW_SIZE), _height(WINDOW_SIZE), _alpha(BIG_ALPHA_ANGLE), _prev_alpha(BIG_ALPHA_ANGLE),
_lightSource1(X_LIGHT1, Y_LIGHT1, 0, 1), _lightSource2(X_LIGHT2, Y_LIGHT2, 0, 1), _origin(0.0, 0.0, 0.0, 1.0)
{
	_edgeX = 1.0f;
	_edgeY = 1.0f;
}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Model::updateBufferVertices()
{
	// Create a triangle fan
	_vertices = createVertices();

	// Copy the vertices to the buffer
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
}

void Model::redefineVertices()
{
	// Bind the buffer and update the vertices
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	updateBufferVertices();
	glBindVertexArray(0);
}

std::vector<float> Model::createVertices()
{
	std::vector<float> vertices;
	float center[] = { _origin.x, _origin.y, _origin.z, _origin.w };
	int theta = 0;

	// Insert the center vertex
	vertices.insert(vertices.begin(), center, center + 4);

	// Add vertices on the perimeter of the circle until we've
	// covered a whole circle
	while (theta <= 360)
	{
		vertices.push_back(RADIUS * cos(theta * (PI / 180)));
		vertices.push_back(RADIUS * sin(theta * (PI / 180)));
		vertices.push_back(0.0f);
		vertices.push_back(1.0f);
		theta += _alpha;
	}

	return vertices;
}

bool Model::isWallCollision(unsigned int ball, bool yAxis, float &offset)
{
	// Calculate the ball position
	glm::vec4 newPos = _ballTransform[ball]._transform * _origin;

	// Get the screen edge for the relevant axis
	float edge = yAxis ? _edgeY : _edgeX;

	if (newPos[(int)yAxis] + _ballTransform[ball]._currentRadius >= edge) // Check right/bottom
	{
		// Calculate the offset needed to fix the position
		offset = -(newPos[(int)yAxis] + _ballTransform[ball]._currentRadius - edge);
	}
	else if (newPos[(int)yAxis] - _ballTransform[ball]._currentRadius <= -edge) // Check left/top
	{
		// Calculate the offset needed to fix the position
		offset = -(newPos[(int)yAxis] - _ballTransform[ball]._currentRadius + edge);
	}
	else
	{
		// No collision
		return false;
	}

	return true;
}

float Model::getBallCollisionRadius(unsigned int ball)
{
	// By default, try using the maximal radius
	float minRadius = _ballTransform[ball]._radius;

	// Iterate over all the balls
	for (unsigned int i = 0; i < _ballTransform.size(); i++)
	{
		if (i == ball)
		{
			// Skip the current ball
			continue;
		}

		// Sum the maximal radii
		float radiusSum = _ballTransform[i]._radius + _ballTransform[ball]._radius;

		// Calculate the ball's position
		glm::vec4 centerBall = _ballTransform[ball]._transform * _origin;

		// Calculate the other ball's position
		glm::vec4 centerI = _ballTransform[i]._transform * _origin;

		// Get the distance between the ball centers
		float distance = glm::length(centerBall - centerI);

		// Check for collision with maximal radii
		if (distance <= radiusSum)
		{
			// Collision with maximal radii, need to adjust

			// Calculate the radio this ball's radius is contributing to the sum of the radii
			float ratio = (_ballTransform[ball]._radius) / radiusSum;

			// Get the distance remaining from this ball's border to the other ball's center
			float rB = std::max(distance - _ballTransform[i]._radius, 0.0f);

			// Get the distance remaining from the i'th ball's border to this ball's center
			float rI = std::max(distance - _ballTransform[ball]._radius, 0.0f);

			// Set the new radius so that the distance between the borders
			// varies linearly according to the ratio
			float r = rB + (distance - rI - rB) * (ratio);

			// Clip at 0
			if (r < 0)
			{
				r = 0;
			}

			// Update the minimal collision radius if smaller
			if (r < minRadius)
			{
				minRadius = r;
			}
		}
	}

	return minRadius;
}

void Model::scaleByWindowSize()
{
	for (unsigned int i = 0; i < _ballTransform.size(); i++)
	{
		glm::vec3 adjust;
		float offset;

		// we reached a wall in the x axis
		if (isWallCollision(i, false, offset))
		{
			adjust.x = offset;
		}
		// we reached a wall in the y axis
		if (isWallCollision(i, true, offset))
		{
			adjust.y = offset;
		}

		// Keep the same direction
		_ballTransform[i]._transform = glm::translate(_ballTransform[i]._transform, adjust);
	}
}

void Model::step()
{
	for (unsigned int i = 0; i < _ballTransform.size(); i++)
	{
		glm::vec3 move = _ballTransform[i]._direction;
		glm::vec3 adjust;
		float offset;

		// we reached a wall in the x axis
		if (isWallCollision(i, false, offset))
		{
			// Go the opposite direction
			move.x *= -1;
			adjust.x = offset;
		}
		// we reached a wall in the y axis
		if (isWallCollision(i, true, offset))
		{
			// Go the opposite direction
			move.y *= -1;
			adjust.y = offset;
		}

		// Update the current radius
		_ballTransform[i]._currentRadius = getBallCollisionRadius(i);

		// Keep the same direction
		_ballTransform[i]._transform = glm::translate(_ballTransform[i]._transform, move + adjust);
		_ballTransform[i]._direction = move;
	}
}

void Model::addBall(int x, int y)
{
	// Generate a random color
	float randR = ((int)(MIN_COLOR * 1000) + rand() % (int)((MAX_COLOR - MIN_COLOR) * 1000)) / 1000.0f;
	float randG = ((int)(MIN_COLOR * 1000) + rand() % (int)((MAX_COLOR - MIN_COLOR) * 1000)) / 1000.0f;
	float randB = ((int)(MIN_COLOR * 1000) + rand() % (int)((MAX_COLOR - MIN_COLOR) * 1000)) / 1000.0f;

	// Choose a random direction
	float theta = (float)(rand() % 360) * PI / 180;

	// Convert the screen coordinates to world coordinates
	glm::vec4 mousePos = glm::vec4(x, y, 0, 1); // Start at (x, y, 0)
	glm::mat4 centered = glm::translate( // Translate so that the origin is at the screen center
		glm::mat4(1.0f),
		glm::vec3(-_width / 2, -_height / 2, 0)
	);
	glm::mat4 scaled = glm::scale( // Scale and flip Y
		glm::mat4(1.0f),
		glm::vec3(_edgeX / (_width / 2), -_edgeY / (_height / 2), 0)
	);
	glm::vec4 pos = scaled * centered * mousePos;

	// Add the ball
	_ballTransform.push_back(BallProperties(
		glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z)),
		glm::vec3(BALL_SPEED * cos(theta), BALL_SPEED * sin(theta), 0.0f),
		glm::vec4(randR, randG, randB, 1.0), RADIUS));

	// Get the distances to the nearest walls
	float distanceX = std::min(glm::length(pos - glm::vec4(1, pos[1], 0, 1)), glm::length(pos - glm::vec4(-1, pos[1], 0, 1)));
	float distanceY = std::min(glm::length(pos - glm::vec4(pos[0], 1, 0, 1)), glm::length(pos - glm::vec4(pos[0], -1, 0, 1)));

	// Get the ball's index (last ball inserted)
	int end = _ballTransform.size() - 1;

	// Get the collision radius
	float distanceBall = getBallCollisionRadius(end);

	// Calculate the maximal radius (minimal distance to walls and other balls)
	float radiusAxis = std::min(distanceX, distanceY);
	float radiusBall = std::min(RADIUS, distanceBall);

	// Set the maximal and current radius
	_ballTransform[end]._radius = _ballTransform[end]._currentRadius = std::min(radiusAxis, radiusBall);

	// Remove the ball if the radius is too small
	if (_ballTransform[end]._radius <= RADIUS_TRESHHOLD)
	{
		_ballTransform.pop_back();
	}
}

void Model::init()
{
	// Add a ball at the screen center
	addBall((int)(_width / 2), (int)(_height / 2));

	programManager::sharedInstance().createProgram(
		"default",
		SHADERS_DIR "SimpleShader.vert",
		SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	// Obtain uniform variable handles:
	_fillColorUV = glGetUniformLocation(program, "fillColor");
	_screenSizeUV = glGetUniformLocation(program, "screenSize");
	_transformUV = glGetUniformLocation(program, "transform");
	_lightSource1UV = glGetUniformLocation(program, "lightSource1");
	_lightSource2UV = glGetUniformLocation(program, "lightSource2");
	_ballCenterUV = glGetUniformLocation(program, "ballCenter");
	_ballRadiusUV = glGetUniformLocation(program, "ballRadius");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		updateBufferVertices();

		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			0,
			0);

		// Unbind vertex array:
		glBindVertexArray(0);

		// Enable blending using source alpha channel
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}


void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL; // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// For scale
	glUniform2f(_screenSizeUV, _width, _height);

	if (_prev_alpha != _alpha)
	{
		redefineVertices();
	}

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	// Rotate the lights
	glm::mat4 lightRot = glm::rotate(glm::mat4(1.0f), (float)(1.0f * PI / 180.0f), glm::vec3(0, 0, 1));
	_lightSource1 = lightRot * _lightSource1;
	_lightSource2 = lightRot * _lightSource2;

	// Pass the light sources' positions
	glUniform2f(_lightSource1UV, _lightSource1.x, _lightSource1.y);
	glUniform2f(_lightSource2UV, _lightSource2.x, _lightSource2.y);

	size_t numberOfVertices = ((360 / _alpha) + 1 + 1);

	for (unsigned int i = 0; i < _ballTransform.size(); i++)
	{
		// Scale by the current radius to maximal radius ratio
		float factor = _ballTransform[i]._currentRadius / RADIUS;
		glm::mat4 scaledRadius = glm::scale(
			glm::mat4(1.0f),
			glm::vec3(factor, factor, 0)
			);

		// Get the final transform
		glm::mat4 transform = _ballTransform[i]._transform * scaledRadius;

		// Calculate the ball's center
		glm::vec4 pos = transform * _origin;

		// Pass the variables to the shaders
		float * fTransform = glm::value_ptr(transform);
		glUniformMatrix4fv(_transformUV, 1, GL_FALSE, fTransform);
		glUniform4fv(_fillColorUV, 1, glm::value_ptr(_ballTransform[i]._color));
		glUniform1f(_ballRadiusUV, _ballTransform[i]._currentRadius);
		glUniform2f(_ballCenterUV, pos.x, pos.y);

		// Draw
		glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	}


	// Unbind the Vertex Array object
	glBindVertexArray(0);

	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::resize(int width, int height)
{
	int size = std::min(width, height);

	_prev_alpha = _alpha;
	_alpha = SMALL_ALPHA_ANGLE;
	if (size >= THRESHOLD_SIZE)
	{
		_alpha = BIG_ALPHA_ANGLE;
	}

	// Update the screen parameters
	_width = (float)width;
	_height = (float)height;
	_offsetX = 0;
	_offsetY = 0;

	// Calculate the new world edges (after scaling for aspect ratio)
	if (width > height)
	{
		_edgeX = _width / _height;
		_edgeY = 1;
	}
	else
	{
		_edgeX = 1;
		_edgeY = _height / _width;
	}

	scaleByWindowSize();
}
