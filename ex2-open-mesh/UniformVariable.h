#ifndef UNIFORM_VARIABLE_H
#define UNIFORM_VARIABLE_H

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

class UniformVariable
{
public:
	UniformVariable();
	virtual ~UniformVariable();

	void init(GLuint program, const char *name);
	virtual void transfer() = 0;

protected:
	GLuint _uv;
};

class FloatUniformVariable : public UniformVariable
{
public:
	void transfer()
	{
		glUniform1f(_uv, value);
	}

	FloatUniformVariable& operator=(const float &value)
	{
		this->value = value;
		return *this;
	}

	float value;
};

class IntUniformVariable : public UniformVariable
{
public:
	void transfer()
	{
		glUniform1i(_uv, value);
	}

	IntUniformVariable& operator=(const int &value)
	{
		this->value = value;
		return *this;
	}

	int value;
};

class Vec3UniformVariable : public UniformVariable
{
public:
	void transfer()
	{
		glUniform3fv(_uv, 1, glm::value_ptr(value));
	}

	Vec3UniformVariable& operator=(const glm::vec3 &value)
	{
		this->value = value;
		return *this;
	}

	glm::vec3 value;
};

class Mat4UniformVariable : public UniformVariable
{
public:
	void transfer()
	{
		glUniformMatrix4fv(_uv, 1, GL_FALSE, glm::value_ptr(value));
	}

	Mat4UniformVariable& operator=(const glm::mat4 &value)
	{
		this->value = value;
		return *this;
	}

	glm::mat4 value;
};

class TextureUniformVariable : public UniformVariable
{
public:
	void transfer()
	{
		glUniform1i(_uv, index);

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	TextureUniformVariable& operator=(const GLint &texture)
	{
		this->texture = texture;
		return *this;
	}

	GLint texture;
	int index;
};

#endif
