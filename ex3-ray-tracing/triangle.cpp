#include "triangle.h"



// Creates a triangle without texture coordinates
Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2)
	: _p0(p0), _u(p1 - p0), _v(p2 - p0), _d((_u % _v).norm()), _textured(false)
{
}

// Creates a triangle with texture coordinates
Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1, Point2d t2)
	: _p0(p0), _u(p1 - p0), _v(p2 - p0), _d((_u % _v).norm()), _t0(t0), _tu(t1), _tv(t2), _textured(true)
{
}

Triangle::~Triangle()
{
}

// Returns which side of the edge is p is 
int Triangle::halfSpace(const Point3d& p, const Point3d& edgeStart, const Point3d& edgeEnd) const
{
	Vector3d  vecEdge = edgeEnd - edgeStart;
	Vector3d  vecEdgeP = p - edgeStart;

	double dotProd = (vecEdge % vecEdgeP) | (_u % _v);

	if (dotProd > 0)
	{
		return 1;
	}

	return -1;
}

// Returns the barycentric point of p
Point2d Triangle::barycentric(const Point3d& p) const
{
	Point3d p1 = _u + _p0;
	Point3d p2 = _v + _p0;

	double lambda0 = ((p1 - p) % (p2 - p)).norm() / _d;
	double lambda1 = ((p - _p0) % _v).norm() / _d;
	double lambda2 = 1 - lambda0 - lambda1;

	return Point2d(lambda0 * _t0[0] + lambda1 * _tu[0] + lambda2 * _tv[0], lambda0 * _t0[1] + lambda1 * _tu[1] + lambda2 * _tv[1]);
}

// Returns true if the given point is inside the triangle
bool Triangle::isInside(IN const Point3d& p, OUT Point2d& texCoord) const
{
	Point3d p1 = _u + _p0;
	Point3d p2 = _v + _p0;

	// On the left, on the left, on the left => p is inside.
	if (halfSpace(p, _p0, p1) > 0 && 
		halfSpace(p, p1,  p2) > 0 &&
		halfSpace(p, p2, _p0) > 0)
	{
		if (_textured)
		{
			texCoord = barycentric(p);
		}
		
		return true;
	}

	return false;
}
