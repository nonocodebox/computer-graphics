
#ifndef _CONE_HH
#define _CONE_HH

//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "sphere.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Cone : public Sphere
{
public:
	// Constructor - create a default sphere  //
	Cone();

	// Constructor - create a cone with the given parameters  //
	Cone(Point3d O, double r, double cutoff);

	// Constructor - create a cone with the given parameters  //
	Cone(Point3d O, double r, double cutoff, Vector3d direction, Vector3d up);

	// Ray cone intersection //
	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

	// Texture mapping (not implemented for the spherical cone) //
	virtual Color3d texture_diffuse(const Point3d& P) const;

private:

	double   _cutoff; // The cone's cutoff angle  //
	Vector3d _direction; // The cone's axis direction //
	Vector3d _up; // The up vector //
};

#endif /* _CONE_HH */
