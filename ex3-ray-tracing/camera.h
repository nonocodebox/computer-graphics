//
//  camera.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Decleration of a camera class, that has the ability to
//            render a scene using the ray tracing algorithm.
//

#ifndef __CAMERA__
#define __CAMERA__


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"


//////////////////////////////
// type Decleration         //
//////////////////////////////

class Scene;
class BImage;

//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Camera
{
public:
  Camera();
	
public:
  // constructor - create a camera with the given paramters
  Camera(Point3d & pos,Point3d & coi, Vector3d & up, double fov, size_t samples_per_pixel = 1);

  // render - create a bitmap image with the given properties from the given scene
  void render(size_t row_start, size_t number_of_rows, BImage& img, Scene & scene) const;

  void setSamplesPerPixel(size_t samples_per_pixel);
	
private:
  Point3d   _position;  // Position of the camera in the 3D space   //
  Point3d   _coi;       // Center of interest for the camera        //
  Vector3d  _up;        // Vector pointing up                       //
  double    _fov_h;     // horizontal field of view for the camera  //
  size_t    _samples_per_pixel;


  struct CameraView
  {
	  Point3d position;
	  Point3d topLeft;
	  Vector3d dx;
	  Vector3d dy;
  };

  // Calculate a view with the given lens offset for the image
  CameraView calculateView(const BImage &img, const Vector3d &offset) const;
};


#endif
