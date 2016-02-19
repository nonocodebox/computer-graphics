//
//  sphere.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a sphere on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the sphere.
//

#ifndef _SPHERE_HH
#define _SPHERE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

static const Vector3d Vn = Vector3d(0.0,1.0,0.0);
static const Vector3d Ve = Vector3d(0.0,0.0,1.0);
static const Vector3d crossVnVe = cross(Vn, Ve);


class Sphere : public Object 
{
public:
  // Create a default sphere
  Sphere();

  // Create a sphere with the given parameters
  Sphere(Point3d C, double r);

  // Returns the sphere's center
  Point3d getCenter() const { return _C; }

  // Returns the sphere's radius
  double getRadius() const { return _r; }

  // Ray Sphere intersection 
  virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

  /* Returns the color of the object at point P according to the texture map and the
     texture coordinates assigned to this object. Returns white in case of any error - no uv coordinates assigned,
     no texture map assigned etc.*/
  virtual Color3d texture_diffuse(const Point3d& P) const;

  // Returns a bounding box for the object
  virtual const BoundingBox & getBoundingBox() const;

protected:
  Point3d   _C; // The sphere's center
  double    _r; // The sphere's radius
  BoundingBox _boundingBox; // The sphere's bounding box

  // Creates the sphere's bounding box
  void createBoundingBox();
};

#endif /* _SPHERE_HH */
