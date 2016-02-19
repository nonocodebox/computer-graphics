#ifndef BVH_H
#define BVH_H

#include "object.h"
#include "bounding_box.h"
#include <vector>

class BVH
{
public:
	BVH();
	~BVH();

	// Add an object to the BVH
	void addObject(Object *obj);

	// Create the BVH binary tree
	void createTree();

	// Intersect a ray with objects in the BVH
	bool findObject(IN Ray ray, IN double tMax, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest) const;
	
private:
	// A single node class
	class Node
	{
	public:
		~Node();

		// Add objects to the node
		void addObjects(std::vector<Object *>::iterator first, std::vector<Object *>::iterator last);
		
		// Intersect a ray with objects in the node
		bool findObject(IN Ray ray, IN double tMax, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest) const;

		// Return the node's bounding box
		const BoundingBox & getBoundingBox() const { return _bbox; }

	private:
		std::vector<Object *> _objects; // The node's leaf objects
		std::vector<Node *> _children; // The node's children
		BoundingBox _bbox; // The node's bounding box
	};

	Node *_root; // The root node
	std::vector<Object *> _objects; // Objects in the BVH
};

#endif
