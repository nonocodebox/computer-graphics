#include "dynamic_object.h"

DynamicObject::DynamicObject()
{
}

// Add a sub-object to the next time unit
void DynamicObject::addObject(Object *object)
{
	_objects.push_back(object);
	_bbox.addBox(object->getBoundingBox());
}

int DynamicObject::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	int time = ray.getTime();

	adjustTime(time);

	return _objects[time]->intersect(ray, tMax, t, P, N, texColor, nearest);
}

Color3d DynamicObject::texture_diffuse(const Point3d& P) const
{
	// Not implemented, use the texture color returned by intersect()
	return Color3d(1, 1, 1);
}

// Returns the bounding box for the object
const BoundingBox & DynamicObject::getBoundingBox() const
{
	return _bbox;
}

// Adjust a given time to be within range
void DynamicObject::adjustTime(int &time) const
{
	if (time < 0)
	{
		time = 0;
	}

	if ((size_t)time >= _objects.size())
	{
		time = _objects.size() - 1;
	}
}