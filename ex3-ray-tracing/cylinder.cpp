#include "cylinder.h"

#include "sphere.h"
#include <cmath>

#define DEFAULT_CENTER_VALUE 0.0
#define DEFAULT_RADIUS 1.0
#define DEFAULT_AXIS_X 1.0
#define DEFAULT_AXIS_Y 0.0
#define DEFAULT_AXIS_Z 0.0
#define DEFAULT_HEIGHT 1.0

Cylinder::Cylinder()
	:_C(DEFAULT_CENTER_VALUE), _axis(DEFAULT_AXIS_X, DEFAULT_AXIS_Y, DEFAULT_AXIS_Z), _r(DEFAULT_RADIUS), _h(DEFAULT_HEIGHT)
{
	createBoundingBox();
}

// Constructor - create a cylinder with the given parameters  //
Cylinder::Cylinder(Point3d C, Vector3d axis, double r)
	: _C(C), _axis(axis.normalized()), _r(r), _h(axis.norm())
{
	createBoundingBox();
}

// Create a bounding box for the cylinder
void Cylinder::createBoundingBox()
{
	_boundingBox.addVertex(_C + Vector3d(_r, _r, _r));
	_boundingBox.addVertex(_C - Vector3d(_r, _r, _r));
	_boundingBox.addVertex(_C + (_axis * _h) + Vector3d(_r, _r, _r));
	_boundingBox.addVertex(_C + (_axis * _h) - Vector3d(_r, _r, _r));
}

// Ray Cylinder intersection //
int Cylinder::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	Vector3d CO = ray.O() - _C;
	Vector3d Dproj = ray.D() - (ray.D() | _axis) * _axis;
	Vector3d COproj = CO - (CO | _axis) * _axis;

	double a = Dproj.sqrnorm();
	double b = 2 * (Dproj | COproj);
	double c = COproj.sqrnorm() - _r * _r;
	double d = b * b - 4 * a * c;

	if (d < 0.0)
	{
		return 0;
	}

	double rootD = sqrt(d);
	double sol1 = 0.5 * (-b - rootD) / a;
	double sol2 = 0.5 * (-b + rootD) / a;
	int result = 0;

	if (sol1 >= EPS && sol1 <= tMax)
	{
		double h = (ray(sol1) - _C) | _axis;

		if (h > EPS && h < _h)
		{
			t = sol1;
			result = 1;
		}
	}

	if ((!result || sol2 < sol1) &&
		sol2 >= EPS && sol2 <= tMax)
	{
		double h = (ray(sol2) - _C) | _axis;

		if (h > EPS && h < _h)
		{
			t = sol2;
			result = 1;
		}
	}

	if (result == 1) // intersetion after origin
	{
		P = ray(t);

		Vector3d CP = P - _C;

		N = CP - (CP | _axis) * _axis;
		N.normalize();

		texColor = texture_diffuse(P);
	}

	return result;
}

/* Returns the color of the object at point P */
/* according to the texture map and the texture coordinates assigned to this object */
/* Returns white in case of any error - no uv coordinates assigned, no texture map assigned etc.*/
Color3d Cylinder::texture_diffuse(const Point3d& P) const
{
	if (_diffuseTexture == NULL)
	{
		return Color3d(1.0, 1.0, 1.0);
	}

	Vector3d CP = P - _C;
	Vector3d CPaxis = (CP | _axis) * _axis;
	Vector3d CPproj = CP - CPaxis;

	Vector3d upWorld(0, 1, 0);

	if ((upWorld % _axis).norm() < EPS)
	{
		upWorld = Vector3d(1, 0, 0);
	}

	Vector3d up = (upWorld - (upWorld | _axis) * _axis).normalize();
	Vector3d right = _axis % up;

	double theta = atan2(CPproj | up, CPproj | right);
	double z = CPaxis.norm();

	double u = (PI + theta) / (2 * PI);
	double v = z / _h;

	u = u < EPS ? EPS : u;
	u = u > (1.0-EPS) ? (1.0-EPS) : u;
	v = v < EPS ? EPS : v;
	v = v > (1.0-EPS) ? (1.0-EPS) : v;

	u *= (_diffuseTexture->getWidth() - 1);
	v *= (_diffuseTexture->getHeight() - 1);

	Bpixel p = (*_diffuseTexture)(u, v);
	return Color3d((double)p.r / 255.0, (double)p.g / 255.0, (double)p.b / 255.0);
}

// Returns a bounding box for the object
const BoundingBox & Cylinder::getBoundingBox() const
{
	return _boundingBox;
}
