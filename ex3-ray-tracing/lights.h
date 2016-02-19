//
//  lights.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Declerae and implement classes representing ambient and
//            point light sources.
//

#ifndef __LIGHTS__
#define __LIGHTS__


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"
#include "sphere.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class AmbientLight 
{
public:
  AmbientLight(Color3d  color = COLOR_BLACK):_color(color){}
public:
  Color3d _color;

};

class Light
{
public:
	virtual ~Light() { };
	virtual Point3d getPosition() const = 0;
	virtual Color3d getColor() const = 0;
};

class PointLight : public Light
{
public:
  PointLight(Point3d position = POINT_ORIGIN,Color3d color = COLOR_BLACK):
    _position(position),_color(color){}

  virtual Point3d getPosition() const { return _position; }
  virtual Color3d getColor() const { return _color; }

  Point3d _position;
  Color3d _color;
};

class SphereLight
	: public Sphere, public Light
{
public:
	SphereLight(Point3d position = POINT_ORIGIN, double radius = 1.0, Color3d color = COLOR_WHITE)
		: Sphere(position, radius), _color(color)
	{
	}

	virtual Point3d getPosition() const { return _C; }
	virtual Color3d getColor() const { return _color; }
	virtual const Light * asLightSource() const { return this; }

protected:
	Color3d _color;
};


#endif
