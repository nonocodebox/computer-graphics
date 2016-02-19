#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

class Cylinder : public Object
{
public:
	// Constructor - create a default cylinder  //
	Cylinder();

	// Constructor - create a cylinder with the given parameters  //
	Cylinder(Point3d C, Vector3d axis, double r);

	Point3d getCenter() const { return _C; }
	Vector3d getAxis() const { return _axis; }
	double getRadius() const { return _r; }

	// Ray Sphere intersection //
	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

	// Returns the object's texture color at point P
	virtual Color3d texture_diffuse(const Point3d& P) const;

	// Retruns a bounding box for the object
	virtual const BoundingBox & getBoundingBox() const;

protected:
	Point3d   _C;    // The cylinder's center  //
	Point3d   _axis; // The cylinder's axis    //
	double    _r;    // The cylinder's radius  //
	double	  _h;    // The cylinder's height  //
	BoundingBox _boundingBox;

	// Create a bounding box for the cylinder
	void createBoundingBox();
};

#endif
