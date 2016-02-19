//
//  Scene2.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//

#ifndef cg_projects_Scene2_h
#define cg_projects_Scene2_h

struct Scene2 : public Scene
{
	
	Scene2()
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
		//PointLight  * p = new PointLight(pos,color);
		SphereLight  * p = new SphereLight(pos, 30.0, color);
		scene.add_light(p);
		
		Point3d pos1(10,20,30);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
		
		
	}
	
	void defineGeometry()
	{
		Scene & scene = *this;
#if !WITHOUT_TEXTURES
		BImage * b = new BImage("textures/checkerboard_lg.bmp");
		BImage * w = new BImage("textures/warning.bmp");
#endif
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1,0,0.0);
		Color3d green(0,1.0,0.0);
		Color3d blue(0,0,1.0);
		
		scene.backgroundColor() = (blue +  white) * 0.5;
		Point3d center(10,0,-2);
		double radius = 4;
		Sphere * sp = new Sphere(center,radius);
		sp->diffuse() = white;
		sp->reflection() = black;
		sp->specular() = white;
		sp->shining() = 16;
#if !WITHOUT_TEXTURES
		sp->set_texture_map(b);
#endif
		scene.add_object(sp);
		
		Point3d center1(0,0,-10);
		double radius1 = 4;
		Sphere * sp1 = new Sphere(center1,radius1);
		sp1->diffuse() = red + green * 0.8;
		sp1->reflection() = white * 0.2;
		sp1->specular() = white;
		sp1->shining() = 256;
		scene.add_object(sp1);
#if !WITHOUT_TEXTURES
		sp1->set_texture_map(w);
#endif
		
		Point3d center2(-10,0,-2);
		double radius2 = 4;
		Sphere * sp2 = new Sphere(center2,radius2);
		sp2->diffuse() = blue;
		sp2->reflection() = white * 0.5;
		sp2->transparency() = white * 0.5;
		sp2->index() = 2.0;
		sp2->specular() = white;
		sp2->shining() = 16;
		scene.add_object(sp2);
		
		
		Point3d center3(0,0,10);
		double radius3 = 4;
		Sphere * sp3 = new Sphere(center3,radius3);
		sp3->diffuse() = white * 0.1;
		sp3->transparency() = white * 0.9;
		sp3->specular() = white;
		sp3->shining() = 64;
		sp3->index() = 1.5;
		scene.add_object(sp3);
		
		
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
		poly->diffuse() = ((blue + red) * 0.5 + white * 0.5) * 0.2;
		poly->reflection() = (blue + red) * 0.5 + white * 0.5;
#if !WITHOUT_TEXTURES
		poly->set_texture_map(b);
#endif
		scene.add_object(poly);
		
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(-1,6,25);
		double fov_h = 50 / 180.0 * M_PI ;
		Point3d coi(0,0,-0);
		Vector3d up(0,1,0) ;
		//		Point3d pos(0,8,35);
		//		double fov_h = 30 / 180.0 * M_PI;
		//		Point3d coi(0,0,-0);
		//		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	
	virtual ~Scene2() {

	}

};


#endif
