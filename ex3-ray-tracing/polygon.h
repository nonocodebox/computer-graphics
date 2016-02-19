//
//  polygon.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a convex polygon on the 3d space.
//            Inherits from Object class, implementing the method to
//            test intersection of a given ray with the polygon.
//

#ifndef _POLYGON_HH
#define _POLYGON_HH

//////////////////////////////
// Library Includes         //
//////////////////////////////

#include <vector>

//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"
#include "triangle.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

using namespace std;

class Polygon : public Object 
{
public:

  // Creates a default polygon
  Polygon();

  // Gets rid of a polygon
  virtual ~Polygon();

  // Creates a polygon from the given vertices
  Polygon(vector<Point3d> & vertices);

  // Creates a polygon from the given vertices, using the given normal
  Polygon(vector<Point3d> & vertices, Vector3d & normal);

  // Creates a polygon from the given vertices, using the given texture map coordinates
  Polygon(vector<Point3d> & vertices, vector<Point2d> textices);

  // Creates a polygon from the given vertices, using the given normal and texture map coordintes
  Polygon(vector<Point3d> & vertices, vector<Point2d> textices, Vector3d & normal);

  // Ray intersection with the convex polygon
  virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

  // Returns the color of the object at point P according to the texture map
  virtual Color3d texture_diffuse(const Point3d& P) const;

  // Returns a bounding box for the object
  virtual const BoundingBox & getBoundingBox() const;

private:
  // Triangulate the polygon into triangles
  void triangulate();
  
  // Calculate the polygon's normal
  void calculateNormal();
  
  // Create the polygon's bounding box
  void createBoundingBox();

  // Ray intersection with a plane
  int intersectPlane(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P);

private:
  vector<Point3d>   _vertices;  // The polygon's vertices                          
  vector<Point2d>   _textices;  // The polygon's texture map coordinates            
  Vector3d          _normal;    // The polygon's normal                            
  bool              _textured;  // Does the polygon have a texture map coordinates
  vector<Triangle*> _triangles; // The polygon's triangles                         
  BoundingBox       _boundingBox; // The polygon's bounding box                            
};

#endif /* _POLYGON_HH */
