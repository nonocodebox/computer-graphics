//
//  object.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Abstract object class, containing the object's colors,
//            reflection and transparency rates, shininess, refraction
//            index, and provides an method to check if a given ray
//            intersects with the object.
//

#ifndef _OBJECT_HH
#define _OBJECT_HH

//////////////////////////////
// Library Includes         //
//////////////////////////////

#include "bimage.h"

//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"
#include "ray.h"
#include "bounding_box.h"


//////////////////////////////
// Constants                //
//////////////////////////////

static const double DEFAULT_SHINING = 25.0; // default shining value for an object    // 
static const double DEFAULT_INDEX   = 1.0;  // default refraction index for an object //

class Light;

#define PI 3.14159265

//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Object
{
public:

  // Default constructor  //
  Object();
  virtual ~Object();

  virtual void set_texture_map(BImage *image) { _diffuseTexture = image; }
  
  // calculate intersection of a given ray with the object //
  virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true) = 0;
  virtual Color3d texture_diffuse(const Point3d& P) const = 0;
  virtual const Light * asLightSource() const { return NULL; }

  Color3d getDiffuse(const Point3d& P) const;

  // Original methods (i left them because the ex3 scene uses them  //
  // and the ex mentioned that we must work with your ex3 file)     //
  Color3d& diffuse();
  Color3d& specular();
  Color3d& ambient();
  Color3d& reflection(); 
  Color3d& transparency();
  double& index();
  double& shining();

  // Const get methods (to let the compiler do the optimizations we want it to do)  //
  const Color3d&  getDiffuse()      const;
  const Color3d&  getSpecular()     const;
  const Color3d&  getAmbient()      const;
  const Color3d&  getReflection()   const;
  const Color3d&  getTransparency() const;
  const double    getIndex()        const;
  const double    getShining()      const;

  // Returns a bounding box for the object
  virtual const BoundingBox& getBoundingBox() const = 0;

protected:
  Color3d _ambient;       // The object's ambient color         //
  Color3d _diffuse;       // The object's diffuse color         //
  Color3d _specular;      // The object's specular color        //
  Color3d _reflection;    // The object's reflection modifier   //
  Color3d _transparency;  // The object's transparency modifier //

  double _shining;        // The object's shining coefficient   //
  double _index ;         // The object's refraction index      //

  BImage* _diffuseTexture;// The objects's diffuse texture map  //
} ;

#endif /* _OBJECT_HH */
