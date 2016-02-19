#include "bounding_box.h"

BoundingBox::BoundingBox()
	: _empty(true)
{
}

// Add a vertex to the bounding box
void BoundingBox::addVertex(const Point3d &vertex)
{
	if (_empty)
	{
		_min = vertex;
		_max = vertex;
		_center = vertex;
		_empty = false;
	}
	else
	{
		_min.minimize(vertex);
		_max.maximize(vertex);
		_center = (_min + _max) / 2;
	}
}

// Add a bounding box (sub-box) to the box
void BoundingBox::addBox(const BoundingBox &box)
{
	addVertex(box.getMin());
	addVertex(box.getMax());
}

// Intersect a ray with the box
bool BoundingBox::intersect(const Ray &r, double tMax) const
{
	double t0 = 0;
	double t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;

	if (r.D()[0] >= 0)
	{
		t_min = (_min[0] - r.O()[0]) / r.D()[0];
		t_max = (_max[0] - r.O()[0]) / r.D()[0];
	}
	else 
	{
		t_min = (_max[0] - r.O()[0]) / r.D()[0];
		t_max = (_min[0] - r.O()[0]) / r.D()[0];
	}

	if (r.D()[1] >= 0) 
	{
		t_y_min = (_min[1] - r.O()[1]) / r.D()[1];
		t_y_max = (_max[1] - r.O()[1]) / r.D()[1];
	}
	else 
	{
		t_y_min = (_max[1] - r.O()[1]) / r.D()[1];
		t_y_max = (_min[1] - r.O()[1]) / r.D()[1];
	}

	if (t_min > t_y_max || t_y_min > t_max)
	{
		return false;
	}
		
	if (t_y_min > t_min)
	{
		t_min = t_y_min;
	}
		
	if (t_y_max < t_max)
	{
		t_max = t_y_max;
	}
		
	if (r.D()[2] >= 0) 
	{
		t_z_min = (_min[2] - r.O()[2]) / r.D()[2];
		t_z_max = (_max[2] - r.O()[2]) / r.D()[2];
	}
	else 
	{
		t_z_min = (_max[2] - r.O()[2]) / r.D()[2];
		t_z_max = (_min[2] - r.O()[2]) / r.D()[2];
	}

	if (t_min > t_z_max || t_z_min > t_max)
	{
		return false;
	}
		
	if (t_z_min > t_min)
	{
		t_min = t_z_min;
	}
		
	if (t_z_max < t_max)
	{
		t_max = t_z_max;
	}

	return ((t_min < tMax) && (t_max > t0));
}
