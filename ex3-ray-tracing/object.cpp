#include "object.h"

Object::Object()
	: _ambient(0.0), _diffuse(0.0), _specular(0.0), _reflection(0.0), _transparency(0.0),
	_shining(0.0), _index(1.0), _diffuseTexture(NULL)
{
}

Object::~Object()
{
}

Color3d Object::getDiffuse(const Point3d& P) const
{
	return _diffuse * texture_diffuse(P);
}

Color3d& Object::diffuse()
{
	return _diffuse;
}

Color3d& Object::specular()
{
	return _specular;
}

Color3d& Object::ambient()
{
	return _ambient;
}

Color3d& Object::reflection()
{
	return _reflection;
}

Color3d& Object::transparency()
{
	return _transparency;
}

double& Object::index()
{
	return _index;
}

double& Object::shining()
{
	return _shining;
}

// Const get methods (to let the compiler do the optimizations we want it to do)  //
const Color3d& Object::getDiffuse() const
{
	return _diffuse;
}

const Color3d& Object::getSpecular() const
{
	return _specular;
}

const Color3d& Object::getAmbient() const
{
	return _ambient;
}

const Color3d& Object::getReflection() const
{
	return _reflection;
}

const Color3d& Object::getTransparency() const
{
	return _transparency;
}

const double Object::getIndex() const
{
	return _index;
}

const double Object::getShining() const
{
	return _shining;
}
