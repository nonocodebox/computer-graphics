#include "MyMeshObject.h"
#include <vector>


MyMeshObject::MyMeshObject(MyMesh & mesh): _mesh(mesh), _boundingSphere(), _center(), _polygons()
{
	_mesh.request_face_normals();
	_mesh.update_normals();

	if (_mesh.has_vertex_texcoords2D())
	{
		_mesh.request_vertex_texcoords2D();
	}

	extractMeshPolygons();
	calculateBounds();
}

void MyMeshObject::set_texture_map(BImage *image)
{
	Object::set_texture_map(image);
	int i = 0;

	for (list<Polygon*>::iterator it = _polygons.begin(); it != _polygons.end(); it++)
	{
		(*it)->set_texture_map(image);
		i++;
	}
}

// Extracts the mesh's polygons
void MyMeshObject::extractMeshPolygons()
{
	vector<Point3d> vertices;
	vector<Point2d> textices;
	MyMesh::FaceHandle fHandle;

	_polygons.clear();

	/* go over all the faces */
	for (MyMesh::FaceIter fIter = _mesh.faces_begin(); fIter != _mesh.faces_end(); ++fIter)
	{
		vertices.clear();
		textices.clear();

		/* for each face - go over all the vertices that belong to it */
		for (MyMesh::FaceVertexIter fvIter = _mesh.fv_iter(fIter); fvIter; ++fvIter)
		{
			vertices.push_back(_mesh.point(fvIter));

			if (_mesh.has_vertex_texcoords2D())
			{
				MyMesh::TexCoord2D tex = _mesh.texcoord2D(fvIter);
				textices.push_back(Point2d(tex));
			}
		}

		MyMesh::Normal n = -_mesh.normal(fIter).normalized();
		Polygon *poly;

		if (_mesh.has_vertex_texcoords2D())
		{
			poly = new Polygon(vertices, textices, n);
		}
		else
		{
			poly = new Polygon(vertices, n);
		}
		
		_polygons.push_back(poly);
		_bvh.addObject(poly);
	}

	_bvh.createTree();
}

// Destructor - free a MyMeshObject //
MyMeshObject::~MyMeshObject()
{
	for (std::list <Polygon *>::iterator it = _polygons.begin(); it != _polygons.end(); it ++)
	{
		delete *it;
	}

	delete _boundingSphere;	 
}

// intersect - calculate intersection of a given ray with the object  //
int MyMeshObject::intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor, IN bool nearest)
{
	double sphereT;
	Point3d sphereP;
	Vector3d sphereN;
	Color3d sphereTexColor;

	// checks whether the ray intersects the bounding sphere 
	int isIntersect = _boundingSphere->intersect(ray, tMax, sphereT, sphereP, sphereN, sphereTexColor, false);

	if (isIntersect == 0)
	{
		return 0;
	}

	Object *obj;

	if (_bvh.findObject(ray, tMax, &obj, t, P, N, texColor, nearest))
	{
		return 1;
	}
	
	return 0;
}

// calculate and create a bounding sphere for the object  //
void MyMeshObject::calculateBounds()
{
	double radius = 0;

	// Vertex iterator is an iterator which goes over all the vertices of the _mesh:
	MyMesh::VertexIter vertexIter;
	// This is used to store the geometrical position of the vertices of the _mesh:
	MyMesh::Point p;

	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
	{
		_boundingBox.addVertex(_mesh.point(*vertexIter));
	}

	// Get the center of the bounding box
	_center = _boundingBox.getCenter();

	// compute the farest point from the center
	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
	{
		// This is how to get the point associated with the vertex:
		p = _mesh.point(vertexIter);
		double currDist = (p - _center).length();

		if(currDist > radius)
		{
			radius = currDist;
		}
	}

	_boundingSphere = new Sphere(_center, radius);
}

// Returns the texture color at point P (not implemented, use intersect())
Color3d MyMeshObject::texture_diffuse(const Point3d& P) const
{
	// Not implemented for MyMeshObject - the color is returned directly from the polygon object's intersect() method
	return Color3d(1, 1, 1);
}

// Returns a bounding box for the object
const BoundingBox & MyMeshObject::getBoundingBox() const
{
	return _boundingBox;
}

