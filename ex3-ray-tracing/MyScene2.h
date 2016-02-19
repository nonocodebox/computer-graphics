
#ifndef MYSCENE2_H
#define MYSCENE2_H

#include "sphere.h"
#include "cylinder.h"
#include "polygon.h"
#include "ellipsoid.h"

struct MyScene2 : public Scene
{

	MyScene2()
		: Scene()
	{
		defineGeometry();
		defineLights();
	}

	void defineLights()
	{
		Scene & scene = *this;

		Point3d pos(16, 13, -14);
		Color3d color(1, 1, 1);
		SphereLight  *deskLight = new SphereLight(pos, 0.5, color);
		deskLight->diffuse() = color;
		deskLight->specular() = color;
		deskLight->shining() = 200;
		scene.add_light(deskLight);

		Point3d topPos(12, 30, -7);
		Color3d topColor(1, 1, 1);
		SphereLight  *topLight = new SphereLight(topPos, 0.5, topColor);
		scene.add_light(topLight);
	}

	void createTable(double h)
	{
		const double tableWidth = 0.5;
		const Color3d tableDiffuse(0.72, 0.48, 0.34);

		vector<Point3d> tableTop1Vertices, tableTop2Vertices, tableBottom1Vertices, tableBottom2Vertices, sideVertices;

		tableTop1Vertices.push_back(Point3d(0, h, 0));
		tableTop1Vertices.push_back(Point3d(6, h, 0));
		tableTop1Vertices.push_back(Point3d(6, h, -17));
		tableTop1Vertices.push_back(Point3d(0, h, -17));

		tableTop2Vertices.push_back(Point3d(6, h, -11));
		tableTop2Vertices.push_back(Point3d(22, h, -11));
		tableTop2Vertices.push_back(Point3d(22, h, -17));
		tableTop2Vertices.push_back(Point3d(6, h, -17));

		Polygon* tableTop1 = new Polygon(tableTop1Vertices);
		Polygon* tableTop2 = new Polygon(tableTop2Vertices);
		tableTop1->diffuse() = tableDiffuse;
		tableTop1->reflection() = COLOR_BLACK;
		tableTop1->specular() = COLOR_BLACK;
		tableTop1->shining() = 1;
		add_object(tableTop1);

		tableTop2->diffuse() = tableTop1->diffuse();
		tableTop2->reflection() = tableTop1->reflection();
		tableTop2->specular() = tableTop1->specular();
		tableTop2->shining() = tableTop1->shining();
		add_object(tableTop2);

		for (size_t i = 0; i < tableTop1Vertices.size(); i++)
		{
			tableBottom1Vertices.push_back(tableTop1Vertices[i] - Vector3d(0, 1, 0) * tableWidth);
		}

		for (size_t i = 0; i < tableTop2Vertices.size(); i++)
		{
			tableBottom2Vertices.push_back(tableTop2Vertices[i] - Vector3d(0, 1, 0) * tableWidth);
		}

		for (size_t i = 0; i < tableTop1Vertices.size(); i++)
		{
			sideVertices.clear();

			size_t j = (i + 1) % tableTop1Vertices.size();

			sideVertices.push_back(tableTop1Vertices[j]);
			sideVertices.push_back(tableTop1Vertices[i]);
			sideVertices.push_back(tableBottom1Vertices[i]);
			sideVertices.push_back(tableBottom1Vertices[j]);

			Polygon *side = new Polygon(sideVertices);

			side->diffuse() = tableTop1->diffuse();
			side->reflection() = tableTop1->reflection();
			side->specular() = tableTop1->specular();
			side->shining() = tableTop1->shining();
			add_object(side);
		}

		for (size_t i = 0; i < tableTop2Vertices.size(); i++)
		{
			sideVertices.clear();

			size_t j = (i + 1) % tableTop2Vertices.size();

			sideVertices.push_back(tableTop2Vertices[j]);
			sideVertices.push_back(tableTop2Vertices[i]);
			sideVertices.push_back(tableBottom2Vertices[i]);
			sideVertices.push_back(tableBottom2Vertices[j]);

			Polygon *side = new Polygon(sideVertices);

			side->diffuse() = tableTop2->diffuse();
			side->reflection() = tableTop2->reflection();
			side->specular() = tableTop2->specular();
			side->shining() = tableTop2->shining();
			add_object(side);
		}

		std::reverse(tableBottom1Vertices.begin(), tableBottom1Vertices.end());
		std::reverse(tableBottom2Vertices.begin(), tableBottom2Vertices.end());

		Polygon* tableBottom1 = new Polygon(tableBottom1Vertices);
		tableBottom1->diffuse() = tableTop1->diffuse();
		tableBottom1->reflection() = tableTop1->reflection();
		tableBottom1->specular() = tableTop1->specular();
		tableBottom1->shining() = tableTop1->shining();
		add_object(tableBottom1);

		Polygon* tableBottom2 = new Polygon(tableBottom2Vertices);
		tableBottom2->diffuse() = tableTop2->diffuse();
		tableBottom2->reflection() = tableTop2->reflection();
		tableBottom2->specular() = tableTop2->specular();
		tableBottom2->shining() = tableTop2->shining();
		add_object(tableBottom2);
	}

	void createDeskLight(double h)
	{
		Point3d lightPos(16, 13, -14);
		Vector3d axis(2, 1, -0.5);
		axis = axis.normalized() * 1.5;
		Cylinder *cyl = new Cylinder(lightPos - axis * 0.3, axis, 1);

		cyl->diffuse() = Color3d(1, 0, 0);
		cyl->reflection() = COLOR_WHITE * 0.5;
		add_object(cyl);

		Sphere *sp = new Sphere(lightPos + axis * 0.7, 1);
		sp->diffuse() = cyl->diffuse();
		sp->reflection() = COLOR_WHITE * 0.5;
		add_object(sp);

		Point3d standPos = lightPos + axis * 0.7;
		Cylinder *stand = new Cylinder(standPos, Vector3d(0, -1, 0) * (standPos[1] - h), 0.1);
		stand->diffuse() = Color3d(0.2, 0.2, 0.2);
		stand->specular() = COLOR_WHITE * 0.2;
		add_object(stand);
	}

	void createWalls(double h, double w, double wzs, double wze, double wys, double wye)
	{
		Color3d diffuse(0.7, 0.7, 0.7);
		vector<Point3d> vertices;
		vector<Point2d> textices;

		// First wall part 1

		vertices.push_back(Point3d(0, 0, 0));
		vertices.push_back(Point3d(0, 0, -17));
		vertices.push_back(Point3d(0, wys, -17));
		vertices.push_back(Point3d(0, wys, 0));

		Polygon *wall1 = new Polygon(vertices);

		wall1->diffuse() = diffuse;
		wall1->reflection() = COLOR_BLACK;
		add_object(wall1);

		// First wall part 2

		vertices.clear();
		vertices.push_back(Point3d(0, wys, 0));
		vertices.push_back(Point3d(0, wys, wzs));
		vertices.push_back(Point3d(0, wye, wzs));
		vertices.push_back(Point3d(0, wye, 0));

		Polygon *wall2 = new Polygon(vertices);

		wall2->diffuse() = diffuse;
		wall2->reflection() = COLOR_BLACK;
		add_object(wall2);

		// First wall part 3

		vertices.clear();
		vertices.push_back(Point3d(0, wys, wze));
		vertices.push_back(Point3d(0, wys, -17));
		vertices.push_back(Point3d(0, wye, -17));
		vertices.push_back(Point3d(0, wye, wze));

		Polygon *wall3 = new Polygon(vertices);

		wall3->diffuse() = diffuse;
		wall3->reflection() = COLOR_BLACK;
		add_object(wall3);

		// First wall part 4

		vertices.clear();
		vertices.push_back(Point3d(0, wye, 0));
		vertices.push_back(Point3d(0, wye, -17));
		vertices.push_back(Point3d(0, h, -17));
		vertices.push_back(Point3d(0, h, 0));

		Polygon *wall4 = new Polygon(vertices);

		wall4->diffuse() = diffuse;
		wall4->reflection() = COLOR_BLACK;
		add_object(wall4);

		// Window border 1

		vertices.clear();
		vertices.push_back(Point3d(0, wys, wzs));
		vertices.push_back(Point3d(0, wys, wze));
		vertices.push_back(Point3d(-w, wys, wze));
		vertices.push_back(Point3d(-w, wys, wzs));

		Polygon *border1 = new Polygon(vertices);

		wall4->diffuse() = diffuse;
		wall4->reflection() = COLOR_BLACK;
		add_object(border1);

		// Window border 2

		vertices.clear();
		vertices.push_back(Point3d(0, wys, wze));
		vertices.push_back(Point3d(0, wye, wze));
		vertices.push_back(Point3d(-w, wye, wze));
		vertices.push_back(Point3d(-w, wys, wze));

		Polygon *border2 = new Polygon(vertices);

		wall4->diffuse() = diffuse;
		wall4->reflection() = COLOR_BLACK;
		add_object(border2);

		// Window border 3

		vertices.clear();
		vertices.push_back(Point3d(0, wye, wze));
		vertices.push_back(Point3d(0, wye, wzs));
		vertices.push_back(Point3d(-w, wye, wzs));
		vertices.push_back(Point3d(-w, wye, wze));

		Polygon *border3 = new Polygon(vertices);

		wall4->diffuse() = diffuse;
		wall4->reflection() = COLOR_BLACK;
		add_object(border3);

		// Window border 4

		vertices.clear();
		vertices.push_back(Point3d(0, wys, wzs));
		vertices.push_back(Point3d(-w, wys, wzs));
		vertices.push_back(Point3d(-w, wye, wzs));
		vertices.push_back(Point3d(0, wye, wzs));

		Polygon *border4 = new Polygon(vertices);

		wall4->diffuse() = diffuse;
		wall4->reflection() = COLOR_BLACK;
		add_object(border4);

		// Second wall

		vertices.clear();
		vertices.push_back(Point3d(0, 0, -17));
		vertices.push_back(Point3d(22, 0, -17));
		vertices.push_back(Point3d(22, h, -17));
		vertices.push_back(Point3d(0, h, -17));

		Polygon *wall5 = new Polygon(vertices);

		wall5->diffuse() = diffuse;
		wall5->reflection() = COLOR_BLACK;
		add_object(wall5);

		vertices.clear();
		vertices.push_back(Point3d(0, wye, wzs));
		vertices.push_back(Point3d(0, wys, wzs));
		vertices.push_back(Point3d(0, wys, wze));
		vertices.push_back(Point3d(0, wye, wze));
		textices.clear();
		textices.push_back(Point2d(0, 0));
		textices.push_back(Point2d(0, 1));
		textices.push_back(Point2d(1, 1));
		textices.push_back(Point2d(1, 0));

		Polygon *window = new Polygon(vertices, textices);

		window->diffuse() = COLOR_WHITE;
		window->specular() = COLOR_WHITE;
		window->reflection() = COLOR_WHITE * 0.1;
		window->shining() = 16;
#if !WITHOUT_TEXTURES
		window->set_texture_map(new BImage("textures/window.bmp"));
#endif
		add_object(window);
	}

	void createTelescope(double h)
	{
		double radius = 0.8;
		double height = 2.5;
		Point3d start(9, h + height, -14);
		Vector3d axis(-5, 2, 2);
		Vector3d axisFlat = axis;

		axisFlat[1] = 0;
		
		Cylinder *tube = new Cylinder(start, axis, radius);
		tube->diffuse() = 0.3 * COLOR_WHITE;
		tube->specular() = 0.2 * COLOR_WHITE;
		tube->reflection() = 0.2 * COLOR_WHITE;
		tube->shining() = 100;
		add_object(tube);

		Ellipsoid *lens = new Ellipsoid(start, radius, radius, radius * 0.3, axis, Vector3d(0, 1, 0));
		lens->diffuse() = Color3d(0, 0, 0.5);
		lens->transparency() = 0.8 * COLOR_WHITE;
		lens->reflection() = 0.2 * COLOR_WHITE;
		lens->index() = 1.5;
		add_object(lens);

		Ellipsoid *lens2 = new Ellipsoid(start + axis, radius, radius, radius * 0.3, axis, Vector3d(0, 1, 0));
		lens2->diffuse() = Color3d(0, 0, 0.5);
		lens2->transparency() = 0.8 * COLOR_WHITE;
		lens2->reflection() = 0.2 * COLOR_WHITE;
		lens2->index() = 1.5;
		add_object(lens2);

		Point3d coneOrigin = start + axis * 0.5 - Vector3d(0, radius, 0);
		Cone *cone = new Cone(coneOrigin, coneOrigin[1] - h, PI / 8, Vector3d(0, -1, 0), Vector3d(1, 0, 0));
		cone->diffuse() = 0.3 * COLOR_WHITE;
		cone->specular() = 0.2 * COLOR_WHITE;
		cone->reflection() = 0.2 * COLOR_WHITE;
		cone->shining() = 100;
		add_object(cone);
	}

	void defineGeometry()
	{
		Scene& scene = *this;
#if !WITHOUT_TEXTURES
		BImage * floorTexture = new BImage("textures/checkerboard_lg.bmp");
		BImage * earthTexture = new BImage("textures/earth.bmp");
#endif
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1.0, 0.0, 0.0);
		Color3d green(0.0, 1.0, 0.0);
		Color3d blue(0.0, 0.0, 1.0);
		Color3d iron(0.30, 0.30, 0.30);

		scene.backgroundColor() = Color3d(0.45, 0.59, 0.61);
		scene.totalTime() = 1;

		double h = 10;

		createTable(h);
		createWalls(h * 3, 1, -8, -14, h * 1.2, h * 1.8);
		createTelescope(h);
		createDeskLight(h);

		// Earth

		double earthRadius = 2;
		Sphere *earth = new Sphere(Point3d(3, h + earthRadius, -5), earthRadius);

		earth->diffuse() = COLOR_WHITE;
		earth->specular() = COLOR_BLACK;
#if !WITHOUT_TEXTURES
		earth->set_texture_map(earthTexture);
#endif
		scene.add_object(earth);

		// Paper

		vector<Point3d> paperVertices;
		vector<Point2d> paperTextices;
		double ph = h + 0.01;
		double pa = -PI / 8;

		Vector3d paperU = Vector3d(cos(pa), 0, sin(pa)) * 2;
		Vector3d paperV = Vector3d(cos(pa + PI / 2), 0, sin(pa + PI / 2)) * 4;
		Point3d paperPos = Point3d(10.5, ph, -16);

		paperVertices.push_back(paperPos);
		paperVertices.push_back(paperPos + paperV);
		paperVertices.push_back(paperPos + paperU + paperV);
		paperVertices.push_back(paperPos + paperU);
		paperTextices.push_back(Point2d(0, 0));
		paperTextices.push_back(Point2d(0, 1));
		paperTextices.push_back(Point2d(1, 1));
		paperTextices.push_back(Point2d(1, 0));

		Polygon *paper = new Polygon(paperVertices, paperTextices);
		paper->diffuse() = COLOR_WHITE;
		paper->specular() = COLOR_BLACK;
#if !WITHOUT_TEXTURES
		paper->set_texture_map(new BImage("textures/paper.bmp"));
#endif
		scene.add_object(paper);

		// Pen

		vector<Point3d> penVertices;
		double penh = h + 0.02;
		double pena = PI / 4;

		Vector3d penU = Vector3d(cos(pena), 0, sin(pena)) * 0.2;
		Vector3d penV = Vector3d(cos(pena + PI / 2), 0, sin(pena + PI / 2)) * 2;
		Point3d penPos = Point3d(14.5, penh, -16);

		penVertices.push_back(penPos);
		penVertices.push_back(penPos + penV);
		penVertices.push_back(penPos + penU + penV);
		penVertices.push_back(penPos + penU);

		Polygon *pen = new Polygon(penVertices);
		pen->diffuse() = Color3d(0, 0, 0.7);
		pen->specular() = COLOR_BLACK;
		scene.add_object(pen);

		//create the floor
		vector<Point3d> plane(4);
		vector<Point2d> plane_uv(4);
		plane[0] = Point3d(0, 0, 0);
		plane[1] = Point3d(22, 0, 0);
		plane[2] = Point3d(22, 0, -17);
		plane[3] = Point3d(0, 0, -17);
		plane_uv[0] = Point2d(0, 0);
		plane_uv[1] = Point2d(0, 1);
		plane_uv[2] = Point2d(1, 1);
		plane_uv[3] = Point2d(1, 0);

		Polygon * floor = new Polygon(plane, plane_uv);
		floor->diffuse() = iron;
		floor->reflection() = white * 0.5;
#if !WITHOUT_TEXTURES
		floor->set_texture_map(floorTexture);
#endif
		scene.add_object(floor);
	}

	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(18, 18, 2);
		double fov_h = 35 / 180.0 * M_PI;
		Point3d coi(8, 12, -13);
		Vector3d up(0, 1, 0);
		camera = Camera(pos, coi, up, fov_h);
	}

	virtual ~MyScene2() {

	}

};


#endif
