#include "bvh.h"
#include <algorithm>

using namespace std;

BVH::BVH()
{
	// Create a root node
	_root = new Node();
}

BVH::~BVH()
{
	// Delete the root node
	delete _root;
}

void BVH::addObject(Object *obj)
{
	// Add the object to the BVH objects
	_objects.push_back(obj);
}

bool sortObjects(Object *first, Object *second)
{
	// Sorts two objects by the bounding box's center Z
	return first->getBoundingBox().getCenter()[2] < second->getBoundingBox().getCenter()[2];
}

void BVH::createTree()
{
	// Sort the objects by the bounding boxes' center Z values
	sort(_objects.begin(), _objects.end(), sortObjects);

	// Add objects to the root node
	_root->addObjects(_objects.begin(), _objects.end());
}

bool BVH::findObject(IN Ray ray, IN double tMax, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest) const
{
	// Intersect the ray with the root node
	return _root->findObject(ray, tMax, object, t, P, N, texColor, nearest);
}

BVH::Node::~Node()
{
	// Delete the node's children
	for (vector<Node *>::iterator it = _children.begin(); it != _children.end(); it++)
	{
		delete *it;
	}
}

void BVH::Node::addObjects(vector<Object *>::iterator first, vector<Object *>::iterator last)
{
	size_t count = last - first;

	if (count <= 2)
	{
		// 2 objects or less, add as leaf nodes
		_objects.insert(_objects.end(), first, last);

		while (first != last)
		{
			_bbox.addBox((*first)->getBoundingBox());
			first++;
		}
	}
	else
	{
		// More then 2 objects, split and add child nodes
		size_t mid = count / 2;

		Node *c1 = new Node();
		Node *c2 = new Node();

		c1->addObjects(first, first + mid);
		c2->addObjects(first + mid, last);

		_children.push_back(c1);
		_children.push_back(c2);

		_bbox.addBox(c1->getBoundingBox());
		_bbox.addBox(c2->getBoundingBox());
	}
}

bool BVH::Node::findObject(IN Ray ray, IN double tMax, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest) const
{
	int intersects;
	double newT;
	Point3d newP;
	Vector3d newN;
	Vector3d newTexColor;

	*object = NULL;

	// Intersect the ray with the leaf objects
	for (vector<Object *>::const_iterator it = _objects.begin(); it != _objects.end(); it++)
	{
		intersects = ((*it)->intersect(ray, tMax, newT, newP, newN, newTexColor, nearest));

		if (intersects && newT > EPS)
		{
			*object = *it;
			tMax = newT;
			t = newT;
			P = newP;
			N = newN;
			texColor = newTexColor;
			
			if (!nearest)
			{
				// Return if not requested nearest object
				return true;
			}
		}
	}

	// Intersect with the child nodes
	for (vector<Node *>::const_iterator it = _children.begin(); it != _children.end(); it++)
	{
		Object *newObj;

		// Intersect with the child's bounding box first
		if ((*it)->getBoundingBox().intersect(ray, tMax))
		{
			// Try finding an actual object
			if ((*it)->findObject(ray, tMax, &newObj, newT, newP, newN, newTexColor, nearest))
			{
				tMax = newT;
				t = newT;
				P = newP;
				N = newN;
				texColor = newTexColor;
				*object = newObj;

				if (!nearest)
				{
					// Return if not requested the nearest object
					return true;
				}
			}
		}
	}

	// Return whether we found an object
	return (*object != NULL);
}
