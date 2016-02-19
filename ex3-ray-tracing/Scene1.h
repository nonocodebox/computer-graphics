//
//  Scene1.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//


#ifndef cg_projects_Scene1_h
#define cg_projects_Scene1_h

struct Scene1 : public Scene
{
	
	Scene1()
	: Scene()
	{
		defineGeometry();
		defineLights();
	}
	
	void defineLights()
	{
		Scene & scene = *this;
		Point3d pos(-30,20,20);
		Color3d color(1,1,1);
		//PointLight  * p = new PointLight(pos,color);
		SphereLight  * p = new SphereLight(pos, 3.0, color);
		scene.add_light(p);
		
		Point3d pos1(30,20,20);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
	}
	
	void defineGeometry()
	{
		Scene& scene = *this;
#if !WITHOUT_TEXTURES
		BImage * b = new BImage("textures/checkerboard_lg.bmp");
		BImage * w = new BImage("textures/warning.bmp");
#endif
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1.0,0.0,0.0);
		Color3d green(0.0,1.0,0.0);
		Color3d blue(0.0,0.0,1.0);
		Color3d iron( 0.30, 0.30, 0.30);
		
		scene.backgroundColor() = (blue +  white) * 0.5;
		
		// First sphere and string  //
		vector<Point3d> string0_vertices(4);
		string0_vertices[0] = Point3d(-11.9, 2.0, -3.9);
		string0_vertices[1] = Point3d(-12.1, 2.0, -4.1);
		string0_vertices[2] = Point3d(-12.1, 21.0, -4.1);
		string0_vertices[3] = Point3d(-11.9, 21.0, -3.9);
		Polygon* string0 = new Polygon(string0_vertices);
		string0->diffuse() = iron;
		string0->reflection() = black;
		string0->specular() = white;
		string0->shining() = 64;
		scene.add_object(string0);
		
		Point3d center0(-12,2,-4);
		double radius = 3;
		Sphere* sp0 = new Sphere(center0,radius);
		sp0->diffuse() = iron;
		sp0->reflection() = white*0.3;
		sp0->specular() = white;
		sp0->shining() = 60;
		scene.add_object(sp0);
		
		// Second sphere and string //
		vector<Point3d> string1_vertices(4);
		string1_vertices[0] = Point3d(-5.9, 2.0, -3.9);
		string1_vertices[1] = Point3d(-6.1, 2.0, -4.1);
		string1_vertices[2] = Point3d(-6.1, 21.0, -4.1);
		string1_vertices[3] = Point3d(-5.9, 21.0, -3.9);
		Polygon* string1 = new Polygon(string1_vertices);
		string1->diffuse() = iron;
		string1->reflection() = black;
		string1->specular() = white;
		string1->shining() = 64;
		scene.add_object(string1);
		
		Point3d center1(-6,2,-4);
		radius = 3;
		Sphere* sp1 = new Sphere(center1,radius);
		sp1->diffuse() = iron;
		sp1->reflection() = white*0.3;
		sp1->specular() = white;
		sp1->shining() = 60;
		scene.add_object(sp1);
		
		// Third sphere and string  //
		vector<Point3d> string2_vertices(4);
		string2_vertices[0] = Point3d(0.05, 2.0, -3.9);
		string2_vertices[1] = Point3d(-0.05, 2.0, -4.1);
		string2_vertices[2] = Point3d(-0.05, 21.0, -4.1);
		string2_vertices[3] = Point3d(0.05,  21.0, -3.9);
		Polygon* string2 = new Polygon(string2_vertices);
		string2->diffuse() = iron;
		string2->reflection() = black;
		string2->specular() = white;
		string2->shining() = 64;
		scene.add_object(string2);
		
		Point3d center2(0,2,-4);
		radius = 3;
		Sphere* sp2 = new Sphere(center2, radius);
		sp2->diffuse() = iron;
		sp2->reflection() = white*0.3;
		sp2->specular() = white;
		sp2->shining() = 60;
#if !WITHOUT_TEXTURES
		sp2->set_texture_map(w);
#endif
		scene.add_object(sp2);
		
		// Fourth sphere and string //
		vector<Point3d> string3_vertices(4);
		string3_vertices[0] = Point3d(5.9, 2.0, -3.9);
		string3_vertices[1] = Point3d(6.1, 2.0, -4.1);
		string3_vertices[2] = Point3d(6.1, 21.0, -4.1);
		string3_vertices[3] = Point3d(5.9, 21.0, -3.9);
		Polygon* string3 = new Polygon(string3_vertices);
		string3->diffuse() = iron;
		string3->reflection() = black;
		string3->specular() = white;
		string3->shining() = 64;
		scene.add_object(string3);
		
		Point3d center3(6,2,-4);
		radius = 3;
		Sphere* sp3 = new Sphere(center3,radius);
		sp3->diffuse() = iron;
		sp3->reflection() = white*0.3;
		sp3->specular() = white;
		sp3->shining() = 60;
		scene.add_object(sp3);
		
		// Fifth sphere and string  //
		vector<Point3d> string4_vertices(4);
		string4_vertices[0] = Point3d(17.4, 4.0, -3.9);
		string4_vertices[1] = Point3d(17.6, 4.0, -4.1);
		string4_vertices[2] = Point3d(12.1, 21.0, -4.1);
		string4_vertices[3] = Point3d(11.9, 21.0, -3.9);
		Polygon* string4 = new Polygon(string4_vertices);
		string4->diffuse() = iron;
		string4->reflection() = black;
		string4->specular() = white;
		string4->shining() = 64;
		scene.add_object(string4);
		
		Point3d center4(17.5,4.0,-4.0);
		radius = 3;
		Sphere* sp4 = new Sphere(center4,radius);
		sp4->diffuse() = iron;
		sp4->reflection() = white*0.3;
		sp4->specular() = white;
		sp4->shining() = 60;
		scene.add_object(sp4);
		
		//create a plane
		vector<Point3d> plane(4);
		vector<Point2d> plane_uv(4);
		double x = 100;
		double z = -4;
		plane[0] = Point3d(-x,z,-x);
		plane[1] = Point3d(-x,z,x);
		plane[2] = Point3d(x,z,x);
		plane[3] = Point3d(x,z,-x);
		plane_uv[0] = Point2d(0,0);
		plane_uv[1] = Point2d(0,1);
		plane_uv[2] = Point2d(1,1);
		plane_uv[3] = Point2d(1,0);
		Polygon * poly = new Polygon(plane,plane_uv);
		poly->diffuse() = iron;
		poly->reflection() = white * 0.5;
#if !WITHOUT_TEXTURES
		poly->set_texture_map(b);
#endif
		scene.add_object(poly);
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(0,8,35);
		double fov_h = 30 / 180.0 * M_PI;
		Point3d coi(0,0,-0);
		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	virtual ~Scene1() {

	}

};


#endif
