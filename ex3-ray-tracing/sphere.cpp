#include "sphere.h"
#include <cmath>

#define DEFAULT_CENTER_VALUE 0.0
#define DEFAULT_RADIUS 1.0

// Creates a sphere with default parameters
Sphere::Sphere()
:_C(DEFAULT_CENTER_VALUE), _r(DEFAULT_RADIUS)
{
	createBoundingBox();
}

// Creates a sphere with the given parameters
Sphere::Sphere(Point3d C, double r)
: _C(C), _r(r)
{
	createBoundingBox();
}

// Creates a bounding box for the sphere
void Sphere::createBoundingBox()
{
	_boundingBox.addVertex(_C + Vector3d(_r, _r, _r));
	_boundingBox.addVertex(_C - Vector3d(_r, _r, _r));
}

// Ray Sphere intersection (See Trirgul slides)
int Sphere::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	Point3d diff = ray.O() - _C;
	double t0 = diff.norm();

	if (t0 - _r > tMax)
	{
		return 0;
	}

	double b = 2 * OpenMesh::dot(diff, ray.D());
	double c = OpenMesh::dot(diff, diff) - _r * _r;

	double d = b*b - 4*c;
	if (d < 0.0)
	{
		return 0;
	}

	double rootD = sqrt(d);
	double sol1 = 0.5 * (-b - rootD);
	double sol2 = 0.5 * (-b + rootD);
	int result = 0;

	if (sol1 >= EPS && sol1 <= tMax)
	{
		t = sol1;
		result = 1;
	}
	else if (sol2 >= EPS && sol1 <= tMax)
	{
		t = sol2;
		result = 1;
	}

	if (result == 1) // intersetion after origin
	{
		P = ray.O() + t * ray.D();
		N = (P - _C);
		N.normalize();

		texColor = texture_diffuse(P);
	}

	return result;
}

/*	Returns the color of the object at point P
	according to the texture map and the texture coordinates assigned to this object 
	Returns white in case of any error - no uv coordinates assigned, no texture map assigned etc.*/
Color3d Sphere::texture_diffuse(const Point3d& P) const
{
	if (_diffuseTexture == NULL)
	{
		return Color3d(1.0, 1.0, 1.0);
	}

	Point3d V = P - _C;

	double theta = atan2(V[0], V[2]);
	double phi = atan2(V[1], sqrt(pow(V[0], 2) + pow(V[2], 2)));

	double u = (theta + PI) / (2 * PI);
	double v = (phi + PI / 2) / PI;

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
const BoundingBox & Sphere::getBoundingBox() const
{
	return _boundingBox;
}
