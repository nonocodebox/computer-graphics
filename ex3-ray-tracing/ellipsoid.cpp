
#include "ellipsoid.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

// Creates a default ellipsoid
Ellipsoid::Ellipsoid()
	:_s(Point3d(0, 0, 0), 1), _C(), _r(1, 1, 1), _direction(1, 0, 0), _up(0, 1, 0)
{
	calculateTransform();
	createBoundingBox();
}

// Creates an ellipsoid with the given parameters
Ellipsoid::Ellipsoid(Point3d C, double a, double b, double c)
	: _s(Point3d(0, 0, 0), 1), _C(C), _r(a, b, c), _direction(1, 0, 0), _up(0, 1, 0)
{
	calculateTransform();
	createBoundingBox();
}

// Creates an ellipsoid with the given parameters
Ellipsoid::Ellipsoid(Point3d C, double a, double b, double c, Vector3d direction, Vector3d up)
	: _s(Point3d(0, 0, 0), 1), _C(C), _r(a, b, c), _direction(direction), _up(up)
{
	calculateTransform();
	createBoundingBox();
}

// Calculates sphere transform
void Ellipsoid::calculateTransform()
{
	double r = _r.min();
	_s = Sphere(Point3d(0, 0, 0), r);
	_transform = glm::mat4(1.0);
	//_transform *= glm::scale(glm::mat4(1.0), glm::vec3(1.0 / _r[0], 1.0 / _r[1], 1.0 / _r[2]));
	_transform *= glm::scale(glm::mat4(1.0), glm::vec3(r / _r[0], r / _r[1], r / _r[2]));
	_transform *= glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(_direction[0], _direction[1], _direction[2]), glm::vec3(_up[0], _up[1], _up[2]));
	_transform *= glm::translate(glm::mat4(1.0), glm::vec3(-_C[0], -_C[1], -_C[2]));
	_invTransform = glm::inverse(_transform);
}

// Calculates a bounding Box
void Ellipsoid::createBoundingBox()
{
	double r = _r.max();

	_boundingBox.addVertex(_C + Vector3d(r, r, r));
	_boundingBox.addVertex(_C - Vector3d(r, r, r));
}

// Ray Transformed (Sphere) intersection
int Ellipsoid::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	double newT;
	Point3d newP;
	Vector3d newN;
	Color3d newTexColor;

	Vector3d newD = transform(_transform, ray.D(), 0.0);
	Ray newRay = Ray(transform(_transform, ray.O(), 1.0), newD, ray.getTime());
	double tFactor = newD.length();

	if (_s.intersect(newRay, tMax * tFactor, newT, newP, newN, newTexColor, nearest))
	{
		newT /= tFactor;

		if (newT < EPS)
		{
			return 0;
		}

		t = newT;
		P = transform(_invTransform, newP, 1.0);
		N = transform(_invTransform, newN, 0.0).normalized();
		texColor = newTexColor;

		return 1;
	}

	return 0;
}

// Returns the color of the object at point P according to the texture map (sphere mapping)
Color3d Ellipsoid::texture_diffuse(const Point3d& P) const
{
	return _s.texture_diffuse(transform(_transform, P, 1.0));
}

// Transforms given point to transformed ellipsoid (sphere) (see Tirgul slides)
Vector3d Ellipsoid::transform(const glm::mat4 &mat, const Vector3d &vec, double w)
{
	glm::vec4 gvec(vec[0], vec[1], vec[2], w);

	gvec = mat * gvec;

	return Vector3d(gvec.x, gvec.y, gvec.z);
}

// Returns a bounding Box for the object
const BoundingBox & Ellipsoid::getBoundingBox() const
{
	return _boundingBox;
}
