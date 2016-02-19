//
//  scene.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class representing a scene, implementing the ray tracing
//            algorithm for color calculation of every ray in the scene.
//

#ifndef _SCENE_HH
#define _SCENE_HH


//////////////////////////////
// Library Includes         //
//////////////////////////////

#ifndef WIN32
#include "bimage.h"
#else
#include "bimage.h"
#endif
#include <vector>


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"
#include "ray.h"
#include "lights.h"
#include "bvh.h"


//////////////////////////////
// Constants                //
//////////////////////////////

static const double MINIMAL_VIS       = 0.1;
static const double RECURSION_FACTOR  = 0.70;

//////////////////////////////
// Class Decleration        //
//////////////////////////////

using std::vector;

class Camera;

class Scene 
{
public:
  // Constructing a 3D scene
  Scene();
  Scene(Color3d & color, AmbientLight & light, double cutoffAngle);

  virtual ~Scene();

  // traceRay - Ray scene intersection for color rays
  Color3d traceRay(Ray ray, double vis = 1.0, double eta = 1.0) const;

  // traceConeRays - Multiple ray-scene intersection after ray perturbation
  Color3d traceConeRays(Ray ray, double vis, double eta, size_t numRays) const;

  // add_object - add the given object to the scene
  void add_object(Object * obj);

  // add_light - add the given light to the scene
  void add_light(PointLight * light);
  void add_light(SphereLight * light);

  // backgroundColor - return reference to the background color
  Color3d & backgroundColor() {return _background;}

  // ambientLight - return reference to the ambient light
  AmbientLight & ambientLight() {return _ambientLight;}

  void setCutoffAngle(double cutoff) { _cutoffAngle = cutoff * PI / 180.0; }
  void setNumberOfRefRays(size_t n) { _numberOfRefRays = n; }

  // The scene's total time
  const int totalTime() const { return _totalTime; }
  int& totalTime() { return _totalTime; }

  virtual void setDefaultCamera(Camera& camer) const = 0;

  // Notifies the scene that the setup is complete and the BVH can be created
  void completeSetup();

  // Returns whether the scene is ready (setup was completed)
  bool isReady() const { return _ready; }

private:
  // find the nearest object intersecting with the given ray
  bool findNearestObject(IN Ray ray, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const;
  
  // calculate the reflection color at the given intersection point
  Color3d calcReflection(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis = 1.0, double eta = 1.0 ,bool isCritical = false) const;

  // calculate the refraction color at the given intersection point
  Color3d calcRefraction(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, bool &reflected, double vis = 1.0, double etaI = 1.0) const;
  Color3d phong(const Ray &ray, const Light &light, const Object &object, const Vector3d &P, const Vector3d& N, const Color3d& texColor) const;

  // Test if the ray intersects any object on its way to the light
  bool shadowTest(Ray& ray, double t_max) const;

  // Returns a ray in a cone with cutoff _cutoff around the input ray r
  Ray perturbateRay(const Ray& r) const;

  // Returns a ray in a cone with cutoff cutoffAngle around the input ray r
  Ray perturbateRay(const Ray& r, double cutoffAngle) const;

  // Returns a ray perturbated by phi and theta
  Ray perturbateRay(const Ray& r, double phi, double theta) const;

private:
  
  vector<PointLight * >   _lights;        // The scene's point lights     //
  vector<SphereLight * >   _sphereLights;        // The scene's point lights     //

  AmbientLight            _ambientLight;  // The scene's Ambient light    //
  Color3d                 _background ;   // The scene's background color //

  Color3d                 _lastReflection;  // The last calculated reflection color //
  
  BVH                     _bvh; // The bounding volume hierarchy //

  double _cutoffAngle;
  double _numberOfRefRays;
  bool _ready;
  int _totalTime;
};

#endif /* _SCENE_HH */
