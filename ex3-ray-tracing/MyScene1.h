
#ifndef MYSCENE1_H
#define MYSCENE1_H

#include "cone.h"
#include "sphere.h"
#include "dynamic_object.h"
#include "cylinder.h"

struct MyScene1 : public Scene
{

	MyScene1()
		: Scene()
	{
		defineGeometry();
		defineLights();
	}

	void defineLights()
	{
		Scene & scene = *this;
		Point3d pos(-30, 20, 20);
		Color3d color(1, 1, 1);
		PointLight  * p = new PointLight(pos, color);
		scene.add_light(p);

		Point3d pos1(30, 20, 20);
		Color3d color1(1, 1, 1);
		PointLight  * p1 = new PointLight(pos1, color1);
		scene.add_light(p1);
	}

	void defineGeometry()
	{
		Scene& scene = *this;
#if !WITHOUT_TEXTURES
		BImage * b = new BImage("textures/checkerboard_lg.bmp");
		BImage * bodyTexture = new BImage("textures/bb8.bmp");
#endif
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1.0, 0.0, 0.0);
		Color3d green(0.0, 1.0, 0.0);
		Color3d blue(0.0, 0.0, 1.0);
		Color3d iron(0.30, 0.30, 0.30);
		Color3d bodyDiffuse(white);
		Color3d headDiffuse(bodyDiffuse * Color3d(0.73, 0.78, 0.86));
		//Color3d bodyDiffuse(0.73, 0.78, 0.86);
		Color3d bodySpecular(white * 0.2);
		Color3d antennaDiffuse(white * 0.4);
		Color3d antennaSpecular(white * 0.1);
		double bodyShining = 100;
		double antennaShining = 50;

		//scene.backgroundColor() = (blue + white) * 0.5;
		scene.backgroundColor() = Color3d(0.45, 0.59, 0.61);
		scene.totalTime() = 10;

		double bodyScale = 0.8;
		double floorLevel = -4;
		double bodyRadius = 10.3 * bodyScale;
		double headRadius = 6 * bodyScale;
		double antenna1Len = 11 * bodyScale;
		double antenna2Len = 9 * bodyScale;
		double eyeRadius = 1.35 * bodyScale;
		double smallEyeRadius = 0.8 * bodyScale;
		double bodyY = floorLevel + bodyRadius;
		double headY = bodyY + bodyRadius * 0.95;
		double eyeTheta = (35.0 / 90.0) * (PI / 2);
		double eyePhi = (-15.0 / 90.0) * (PI / 2);
		double eyeY = headY + sin(eyeTheta) * headRadius;
		double eyeX = sin(eyePhi) * cos(eyeTheta) * headRadius;
		double eyeZ = cos(eyePhi) * cos(eyeTheta) * headRadius;
		double smallEyeTheta = (20.0 / 90.0) * (PI / 2);
		double smallEyePhi = (5.0 / 90.0) * (PI / 2);
		double smallEyeY = headY + sin(smallEyeTheta) * headRadius;
		double smallEyeX = sin(smallEyePhi) * cos(smallEyeTheta) * headRadius;
		double smallEyeZ = cos(smallEyePhi) * cos(smallEyeTheta) * headRadius;
		Point3d headCenter(0, headY, 0);
		Point3d bodyCenter(0, bodyY, 0);
		Point3d eyeCenter(eyeX, eyeY, eyeZ);
		Point3d smallEyeCenter(smallEyeX, smallEyeY, smallEyeZ);
		Point3d antenna1Start(headRadius * 0.1, headY, -0.5 * headRadius);
		Point3d antenna2Start(-headRadius * 0.1, headY, -0.5 * headRadius);
		double antennaWidth = headRadius * 0.03;

		Cone* head = new Cone(headCenter, headRadius, (80.0 / 90.0) * (PI / 2), Vector3d(0, 1, 0), Vector3d(1, 0, 0));
		head->diffuse() = headDiffuse;
		head->reflection() = black;
		head->specular() = bodySpecular;
		head->shining() = bodyShining;
		scene.add_object(head);

		DynamicObject* dynamicBody = new DynamicObject();

		dynamicBody->diffuse() = bodyDiffuse;
		dynamicBody->reflection() = black;
		dynamicBody->specular() = bodySpecular;
		dynamicBody->shining() = bodyShining;

		for (int i = 0; i < scene.totalTime(); i++)
		{
			Sphere* body = new Sphere(bodyCenter + 0.2 * (double)(i - scene.totalTime() / 2) * Vector3d(-1, 0, 1), bodyRadius);
			
			body->diffuse() = bodyDiffuse;
			body->reflection() = black;
			body->specular() = bodySpecular;
			body->shining() = bodyShining;
#if !WITHOUT_TEXTURES
			body->set_texture_map(bodyTexture);
#endif

			dynamicBody->addObject(body);
		}

		scene.add_object(dynamicBody);

		Sphere* eye = new Sphere(eyeCenter, eyeRadius);
		eye->diffuse() = white * 0.05;
		eye->reflection() = white * 0.1;
		eye->specular() = white * 0.5;
		eye->shining() = 200;
		scene.add_object(eye);

		Sphere* smallEye = new Sphere(smallEyeCenter, smallEyeRadius);
		smallEye->diffuse() = white * 0.05;
		smallEye->reflection() = white * 0.1;
		smallEye->specular() = white * 0.5;
		smallEye->shining() = 200;
		scene.add_object(smallEye);

		vector<Point3d> antenna1Vertices;
		antenna1Vertices.push_back(Point3d(antenna1Start[0] - antennaWidth / 2, antenna1Start[1], antenna1Start[2]));
		antenna1Vertices.push_back(Point3d(antenna1Start[0] + antennaWidth / 2, antenna1Start[1], antenna1Start[2]));
		antenna1Vertices.push_back(Point3d(antenna1Start[0] + antennaWidth / 2, antenna1Start[1] + antenna1Len, antenna1Start[2]));
		antenna1Vertices.push_back(Point3d(antenna1Start[0] - antennaWidth / 2, antenna1Start[1] + antenna1Len, antenna1Start[2]));

		Polygon* antenna1 = new Polygon(antenna1Vertices);
		antenna1->diffuse() = antennaDiffuse;
		antenna1->reflection() = black;
		antenna1->specular() = antennaSpecular;
		antenna1->shining() = antennaShining;
		scene.add_object(antenna1);

		vector<Point3d> antenna2Vertices;
		antenna2Vertices.push_back(Point3d(antenna2Start[0] - antennaWidth / 2, antenna2Start[1], antenna2Start[2]));
		antenna2Vertices.push_back(Point3d(antenna2Start[0] + antennaWidth / 2, antenna2Start[1], antenna2Start[2]));
		antenna2Vertices.push_back(Point3d(antenna2Start[0] + antennaWidth / 2, antenna2Start[1] + antenna2Len, antenna2Start[2]));
		antenna2Vertices.push_back(Point3d(antenna2Start[0] - antennaWidth / 2, antenna2Start[1] + antenna2Len, antenna2Start[2]));

		Polygon* antenna2 = new Polygon(antenna2Vertices);
		antenna2->diffuse() = antennaDiffuse;
		antenna2->reflection() = black;
		antenna2->specular() = antennaSpecular;
		antenna2->shining() = antennaShining;
		scene.add_object(antenna2);

		//create a plane
		vector<Point3d> plane(4);
		vector<Point2d> plane_uv(4);
		double x = 100;
		double z = floorLevel;
		plane[0] = Point3d(-x, z, -x);
		plane[1] = Point3d(-x, z, x);
		plane[2] = Point3d(x, z, x);
		plane[3] = Point3d(x, z, -x);
		plane_uv[0] = Point2d(0, 0);
		plane_uv[1] = Point2d(0, 1);
		plane_uv[2] = Point2d(1, 1);
		plane_uv[3] = Point2d(1, 0);

		Polygon * poly = new Polygon(plane, plane_uv);
		poly->diffuse() = iron;
		poly->reflection() = white * 0.5;
#if !WITHOUT_TEXTURES
		poly->set_texture_map(b);
#endif
		
		scene.add_object(poly);
	}

	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(0, 12, 35);
		double fov_h = 30 / 180.0 * M_PI;
		Point3d coi(0, 4, -0);
		Vector3d up(0, 1, 0);
		camera = Camera(pos, coi, up, fov_h);
	}
	virtual ~MyScene1() {

	}

};


#endif
