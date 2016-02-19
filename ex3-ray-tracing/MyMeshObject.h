//
//  MyMeshObject.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Decleration of an object, made out of a Mesh.
//

#ifndef __MY_MESH_OBJECT_
#define __MY_MESH_OBJECT_

//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyMesh.h"
#include "object.h"
#include "polygon.h"
#include "sphere.h"
#include "bvh.h"
#include  <list>


//////////////////////////////
// Type defintions          //
//////////////////////////////

class Sphere;


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class MyMeshObject : public Object
{
public:
  // Constructor - create a MyMeshObject from the given mesh  //
  MyMeshObject(MyMesh & mesh);

  // Destructor - free a MyMeshObject //
  virtual ~MyMeshObject();

  virtual void set_texture_map(BImage *image);

  // intersect - calculate intersection of a given ray with the object  //
  virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

  // Returns the texture color at point P (not implemented, use intersect())
  Color3d texture_diffuse(const Point3d& P) const;

  // Returns a bounding box for the object
  virtual const BoundingBox & getBoundingBox() const;

private:
  // calculate and create a bounding sphere and box for the object  //
  void calculateBounds();

  // adding each polygon of the mesh model to a list       //
  void extractMeshPolygons();

private:
  MyMesh    _mesh;                      // The mesh the object is made of   //
  Sphere*   _boundingSphere;            // The sphere bounding the object   //
  Point3d   _center;                    // The center of the mesh           //
  std::list <Polygon *> _polygons;      // A list of all the polygons      //
  BoundingBox _boundingBox;
  BVH _bvh;
};


#endif
