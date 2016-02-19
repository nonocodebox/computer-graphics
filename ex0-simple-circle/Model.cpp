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

#define PI					3.1415926535897
#define BIG_ALPHA_ANGLE		10
#define SMALL_ALPHA_ANGLE	24
#define THRESHOLD_SIZE		400
#define RADIUS				0.5f
#define WINDOW_SIZE			600

#define SHADERS_DIR "shaders/"

Model::Model() :
_vao(0), _vbo(0), _width(WINDOW_SIZE), _height(WINDOW_SIZE), _alpha(BIG_ALPHA_ANGLE), _prev_alpha(BIG_ALPHA_ANGLE)
{
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
	std::vector<float> vertices = createVertices();

	// Copy the vertices to the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
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
	float center[] = { 0.0f, 0.0f, 0.0f, 1.0f };
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

void Model::init()
{
	programManager::sharedInstance().createProgram(
				"default",
				SHADERS_DIR "SimpleShader.vert",
				SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	// Obtain uniform variable handles:
	_fillColor1UV  = glGetUniformLocation(program, "fillColor1");
	_fillColor2UV  = glGetUniformLocation(program, "fillColor2");
	_screenSizeUV = glGetUniformLocation(program, "screenSize");


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
	}
}


void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL; // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Set uniform variable with RGB values:
	float red = 0.3f; float green = 0.5f; float blue = 0.7f;
	glUniform4f(_fillColor1UV, 0.0, 0.0, 0.0, 1.0);
	glUniform4f(_fillColor2UV, red, green, blue, 1.0);
	// For scale
	glUniform2f(_screenSizeUV, _width, _height);

	if (_prev_alpha != _alpha)
	{
		redefineVertices();
	}

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	size_t numberOfVertices = ((360 / _alpha) + 1 + 1);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

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
	
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}
