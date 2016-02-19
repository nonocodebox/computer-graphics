#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "object.h"
#include <vector>

class DynamicObject : public Object
{
public:
	DynamicObject();

	// Add a sub-object to the next time unit
	void addObject(Object *object);

	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest = true);

	// Not implemented, use the texture color returned by intersect()
	virtual Color3d texture_diffuse(const Point3d& P) const;

	// Returns a bounding box for the object
	virtual const BoundingBox & getBoundingBox() const;

private:
	std::vector<Object *> _objects;
	BoundingBox _bbox;

	// Adjust a given time to be within range
	void adjustTime(int &time) const;
};

#endif
