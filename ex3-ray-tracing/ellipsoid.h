//
//  ellipsoid.h
//
//  Purpose : A class that represents an ellipsoid on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the ellipsoid.
//

#ifndef _ELLIPSOID_HH
#define _ELLIPSOID_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "sphere.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Ellipsoid : public Object
{
public:
	// Creates a default ellipsoid
	Ellipsoid();

	// Creates an ellipsoid with the given parameters
	Ellipsoid(Point3d C, double a, double b, double c);

	// Create an ellipsoid with the given parameters
	Ellipsoid(Point3d C, double a, double b, double c, Vector3d direction, Vector3d up);

	// Ray ellipsoid intersection
	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

	// Returns the color of the object at point P according to the texture map
	virtual Color3d texture_diffuse(const Point3d& P) const;

	// Returns a bounding box for the object
	virtual const BoundingBox & getBoundingBox() const;

private:

	glm::mat4 _transform, _invTransform;
	Sphere _s;
	Point3d _C;
	Vector3d _r, _direction, _up;
	BoundingBox _boundingBox;

	// Calculates sphere transform
	void calculateTransform();

	// Calculates a bounding Box
	void createBoundingBox();

	// Transforms given point to transformed ellipsoid (sphere) (see Tirgul slides)
	static Vector3d transform(const glm::mat4 &mat, const Vector3d &vec, double w);
};

#endif /* _ELLIPSOID_HH */
