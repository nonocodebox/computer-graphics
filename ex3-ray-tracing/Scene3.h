//
//  Scene3.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//

#ifndef cg_projects_Scene3_h
#define cg_projects_Scene3_h

struct Scene3 : public Scene
{
	Scene3()
	: Scene()
	{
		defineGeometry();
		defineLights();
	}
	
	void defineLights()
	{
		Scene & scene = *this;
		Point3d pos(10,100,10);
		Color3d color(1,1,1);
		//PointLight  * p = new PointLight(pos, color);
		SphereLight  * p = new SphereLight(pos, 30.0, color);
		scene.add_light(p);
		
		Point3d pos1(10,20,-10);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
	}
	
	void defineGeometry()
	{
		Scene & scene = *this;
#if !WITHOUT_TEXTURES
		BImage * b = new BImage("textures/checkerboard_lg.bmp");
		BImage * w = new BImage("textures/face_uv.bmp");
#endif
		
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1,0,0.0);
		Color3d green(0,1.0,0.0);
		Color3d blue(0,0,1.0);
		
		
		Point3d center(-2,0,2);
		double radius = 1;
		Sphere * sp = new Sphere(center,radius);
		sp->diffuse() = white;
		sp->reflection() = black;
		sp->specular() = white;
		sp->shining() = 16;
#if !WITHOUT_TEXTURES
		sp->set_texture_map(b);
#endif
		scene.add_object(sp);
		
		scene.backgroundColor() = (blue +  white) * 0.5;
		
		MyMesh mesh;
		mesh.request_vertex_texcoords2D();
		if (!OpenMesh::IO::read_mesh(mesh, "meshes/girl_face.obj"))
		{
			fprintf(stderr, "Error: cannot read mesh from file\n");
			exit(-1);
		}
		MyMeshObject * mo = new MyMeshObject(mesh);
		mo->diffuse() = white;
		mo->reflection() = black;
		//  mo->transparency() = white * 0.5;
		mo->specular() = black;
		mo->shining() = 0;
		mo->index() = 1.0;
#if !WITHOUT_TEXTURES
		mo->set_texture_map(w);
#endif
		scene.add_object(mo);
		
		//create a plane
		vector<Point3d> plane(4);
		double x = 100;
		double z = -4;
		plane[0] = Point3d(-x,z,-x);
		plane[1] = Point3d(-x,z,x);
		plane[2] = Point3d(x,z,x);
		plane[3] = Point3d(x,z,-x);
		Polygon * poly = new Polygon(plane);
		poly->diffuse() = ((blue + red) * 0.5 + white * 0.5) * 0.2;
		poly->reflection() = (blue + red) * 0.5 + white * 0.5;
		scene.add_object(poly);
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(1,0.5,-4);
		double fov_h = 30 / 180.0 * M_PI ;
		Point3d coi(0,0,-0);
		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	
	virtual ~Scene3() {

	}

};

#endif
