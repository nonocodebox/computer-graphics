#include "cone.h"
#include <cmath>

#define DEFAULT_CUTOFF (PI / 4)

Cone::Cone()
	: Sphere(), _cutoff(DEFAULT_CUTOFF), _direction(1, 0, 0), _up(0, 1, 0)
{
}

// Constructor - create a cone with the given parameters  //
Cone::Cone(Point3d O, double r, double cutoff)
	: Sphere(O, r), _cutoff(cutoff), _direction(1, 0, 0), _up(0, 1, 0)
{
}

// Constructor - create a cone with the given parameters  //
Cone::Cone(Point3d O, double r, double cutoff, Vector3d direction, Vector3d up)
	: Sphere(O, r), _cutoff(cutoff), _direction(direction), _up(up)
{
}

// Ray Sphere intersection //
int Cone::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	double sphereT;
	Point3d sphereP;
	Vector3d sphereN;
	Color3d sphereTexColor;

	if (!Sphere::intersect(ray, tMax, sphereT, sphereP, sphereN, sphereTexColor, nearest))
	{
		return 0;
	}

	double cos2 = pow(cos(_cutoff), 2);
	Point3d l = ray.O() - _C;
	double a = pow((_direction | ray.D()), 2) - pow(ray.D().norm(),2)*cos2;
	double b = 2 * (_direction | l)*(_direction|ray.D()) - 2*(ray.D() | l)*cos2;
	double c = -pow(l.norm(),2)*cos2 + pow((_direction | l), 2);
	double d = b * b - 4 * a * c;

	if (d < 0.0)
	{
		return 0;
	}

	double rootD = sqrt(d);
	double sol1 = 0.5 * (-b - rootD) / a;
	double sol2 = 0.5 * (-b + rootD) / a;

	int result = 0;
	Vector3d v1 = ray.O() + sol1 * ray.D() - _C;
	Vector3d v2 = ray.O() + sol2 * ray.D() - _C;
	Vector3d vSphere = ray.O() + sphereT * ray.D() - _C;

	if (sol1 >= EPS && sol1 <= tMax && 
		(v1 | _direction) >= 0 && v1.norm() < _r)
	{
		t = sol1;
		result = 1;
	}
	
	if ((!result || sol2 < sol1) && 
		sol2 >= EPS && sol2 <= tMax && 
		(v2 | _direction) >= 0 && v2.norm() < _r)
	{
		t = sol2;
		result = 1;
	}

	if ((vSphere.normalized() | _direction) >= cos(_cutoff) && (!result || sphereT < t))
	{
		t = sphereT;
		P = sphereP;
		N = sphereN;
		texColor = sphereTexColor;

		return 1;
	}

	if (result == 1)
	{
		P = ray.O() + t * ray.D();
		Vector3d cp = P - _C;
		Vector3d pd = (cp | _direction) * _direction;
		Vector3d pr = cp - pd;
		N = (pd + pr).normalized();

		texColor = texture_diffuse(P);
	}

	return result;
}

/* Returns the color of the object at point P */
/* according to the texture map and the texture coordinates assigned to this object */
/* Returns white in case of any error - no uv coordinates assigned, no texture map assigned etc.*/
Color3d Cone::texture_diffuse(const Point3d& P) const
{
	// Texture mapping is not implemented for the spherical cone
	return Color3d(1.0, 1.0, 1.0);
}
