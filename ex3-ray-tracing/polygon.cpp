#include "polygon.h"
#include <cmath>
//
//vector<Point3d>   _vertices;  // The polygon's vertices                           //
//vector<Triangle*> _triangles; // The polygon's triangles                          //
//Vector3d          _normal;
//bool              _textured;  // Does the polygon have a texture map coordinates  //
//vector<Point2d>   _textices;  // The polygon's texture map coordinates            //
// Constructor - Create a polygon from the given vertices //

// Creates a polygon given veritces
Polygon::Polygon(vector<Point3d> & vertices)
	: _vertices(vertices), _textured(false)
{
	calculateNormal();
	triangulate();
	createBoundingBox();
}

// Creates a polygon from the given vertices, using the given normal
Polygon::Polygon(vector<Point3d> & vertices, Vector3d & normal)
	: _vertices(vertices), _normal(normal), _textured(false)
{
	triangulate();
	createBoundingBox();
}

// Creates a polygon from the given vertices, using the given texture map coordinates //
Polygon::Polygon(vector<Point3d> & vertices, vector<Point2d> textices)
	: _vertices(vertices), _textices(textices), _textured(true)
{
	calculateNormal();
	triangulate();
	createBoundingBox();
}

// Constructor - Create a polygon from the given vertices, using the given normal and texture map coordintes //
Polygon::Polygon(vector<Point3d> & vertices, vector<Point2d> textices, Vector3d & normal)
	: _vertices(vertices), _textices(textices), _normal(normal), _textured(true)
{
	triangulate();
	createBoundingBox();
}

// Destructs triangles
Polygon::~Polygon()
{
	for (unsigned int i = 0; i < _triangles.size(); i++)
	{
		delete _triangles[i];
	}
}

// Ray intersection with a plane
int Polygon::intersectPlane(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P)
{
	double d = -_normal | _vertices[0];
	double dDotN = ray.D() | _normal;

	if (fabs(dDotN) < EPS)
	{
		return 0;
	}
	
	t = -((ray.O() | _normal) + d) / (ray.D() | _normal);
	P = ray.O() + t * ray.D();

	if (t > tMax || t < EPS)
	{
		return 0;
	}

	return 1;
}


// Ray intersection with the convex polygon //
int Polygon::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	Point2d texCoord;
	Point3d planeP;
	double planeT;

	// Check if the ray intersects the polygon's plane
	if (!intersectPlane(ray, tMax, planeT, planeP))
	{
		return 0;
	}

	// Test each triangle
	for (unsigned int i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i]->isInside(planeP, texCoord))
		{
			if (_diffuseTexture != NULL && _textured)
			{
				// Calculate the texture coordinate
				texCoord[0] = texCoord[0] < 0 ? 0 : texCoord[0];
				texCoord[0] = texCoord[0] > 1 ? 1 : texCoord[0];
				texCoord[1] = texCoord[1] < 0 ? 0 : texCoord[1];
				texCoord[1] = texCoord[1] > 1 ? 1 : texCoord[1];

				texCoord[0] *= ((*_diffuseTexture).getWidth() - 1);
				texCoord[1] *= ((*_diffuseTexture).getHeight() - 1);

				// Get the texture color
				Bpixel color = (*_diffuseTexture)(texCoord[0], texCoord[1]);
				texColor = Color3d((double)color.r / 255.0, (double)color.g / 255.0, (double)color.b / 255.0);
			}
			else
			{
				texColor = Color3d(1, 1, 1);
			}

			t = planeT; // Intersection t
			P = planeP; // Intersection point
			N = _normal; // Normal at intersection point

			return 1;
		}
	}

	// No intersection
	return 0;
}

// Triangulate the polygon into triangles
void Polygon::triangulate()
{
	_triangles.clear();

	if (_textured)
	{
		// Add textured triangles
		for (unsigned int i = 0; i < _vertices.size() - 2; i++)
		{
			_triangles.push_back(new Triangle(_vertices[0], _vertices[i + 1], _vertices[i + 2], _textices[0], _textices[i + 1], _textices[i + 2]));
		}
	}
	else
	{
		// Add untextured triangles
		for (unsigned int i = 0; i < _vertices.size() - 2; i++)
		{
			_triangles.push_back(new Triangle(_vertices[0], _vertices[i + 1], _vertices[i + 2]));
		}
	}
}

// Calculate the polygon's normal
void Polygon::calculateNormal()
{
	_normal = ((_vertices[1] - _vertices[0]) % (_vertices[2] - _vertices[0])).normalized();
}

// Returns the color of the object at point P according to the texture map
Color3d Polygon::texture_diffuse(const Point3d& P) const
{
	Point2d texCoord;

	// Check if the polygon is textured
	if (_diffuseTexture == NULL || !_textured)
	{
		return Color3d(1, 1, 1);
	}

	// Check each triangle
	for (unsigned int i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i]->isInside(P, texCoord))
		{
			texCoord[0] = texCoord[0] < 0 ? 0 : texCoord[0];
			texCoord[0] = texCoord[0] > 1 ? 1 : texCoord[0];
			texCoord[1] = texCoord[1] < 0 ? 0 : texCoord[1];
			texCoord[1] = texCoord[1] > 1 ? 1 : texCoord[1];

			texCoord[0] *= ((*_diffuseTexture).getWidth() - 1);
			texCoord[1] *= ((*_diffuseTexture).getHeight() - 1);

			Bpixel color = (*_diffuseTexture)(texCoord[0], texCoord[1]);
			return Color3d((double)color.r / 255.0, (double)color.g / 255.0, (double)color.b / 255.0);
		}
	}

	return Color3d(1, 1, 1);
}

// Create the polygon's bounding box
void Polygon::createBoundingBox()
{
	for (vector<Point3d>::iterator it = _vertices.begin(); it != _vertices.end(); it++)
	{
		_boundingBox.addVertex(*it);
	}
}

// Returns a bounding box for the object
const BoundingBox & Polygon::getBoundingBox() const
{
	return _boundingBox;
}
