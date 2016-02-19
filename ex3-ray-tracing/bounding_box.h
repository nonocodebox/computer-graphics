#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "MyVecs.h"
#include "ray.h"

class BoundingBox
{
public:
	BoundingBox();

	// Add a vertex to the bounding box
	void addVertex(const Point3d &vertex);

	// Add a bounding box (sub-box) to the box
	void addBox(const BoundingBox &box);

	// Get the minimum, maximum and center of the box
	const Point3d& getMin() const { return _min; }
	const Point3d& getMax() const { return _max; }
	const Point3d& getCenter() const { return _center; }

	// Intersect a ray with the box
	bool intersect(const Ray &r, double tMax) const;

private:
	Point3d _min, _max, _center;
	bool _empty;
};
#endif
