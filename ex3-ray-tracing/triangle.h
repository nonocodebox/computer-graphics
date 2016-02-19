//
//  triangle.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a triangle on the 3d space.
//            Implements a method checking if a given point is inside
//            the triangle.
//

#ifndef _TRIANGLE_HH
#define _TRIANGLE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Triangle 
{
public:
  // Creates a triangle from the given points
  Triangle(Point3d p0, Point3d p1, Point3d p2);

  // Creates a triangle from default points
  Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1, Point2d t2);

  // Gets rid of a triangle
  ~Triangle();

  // Returns true if the given point is inside the triangle
  bool isInside(IN const Point3d& p, OUT Point2d& texCoord) const;

private:
  Point3d _p0;  // One of the triangle vertices
  Vector3d _u;  // a vector from p0 to p1   
  Vector3d _v;  // a vector from p0 to p2         

  double _d; // The plane's D value

  Point2d _t0;  // The texture map coordiate of p0
  Point2d _tu;  // The texture map coordinate of p1
  Point2d _tv;  // The texture map coordinate of p2

  bool  _textured; // Whether the triangle is textured

  // Returns which side of the edge is p is
  int halfSpace(const Point3d& p, const Point3d& edgeStart, const Point3d& edgeEnd) const;

  // Returns the barycentric point of p
  Point2d barycentric(const Point3d& p) const;
};

#endif /* _TRIANGLE_HH */
