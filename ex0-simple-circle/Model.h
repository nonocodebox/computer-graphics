//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

class Model {
	
	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColor1UV, _fillColor2UV;
	GLint _screenSizeUV;
	
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	int _alpha;
	int _prev_alpha;

public:
	Model();

public:
	virtual ~Model();

public:
	void init();
	
public:
	void draw();
	
public:
	void resize(int width, int height);
private:
	std::vector<float> createVertices();
	void redefineVertices();
	void updateBufferVertices();
};

#endif /* defined(__ex0__Model__) */
