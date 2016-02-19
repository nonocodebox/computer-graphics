#include "scene.h"
#include <limits>

#define NUM_SHADOWS			20

Scene::Scene()
	: _numberOfRefRays(1),_ready(false), _totalTime(1)
{
	srand(0);
	_cutoffAngle = 5.0 * PI / 180.0;
}

Scene::Scene(Color3d & color, AmbientLight & light, double cutoffAngle)
	: _ambientLight(light), _background(color), _cutoffAngle(cutoffAngle), _numberOfRefRays(1), _ready(false), _totalTime(1)
{
}

// Notifies the scene that the setup is complete and the BVH can be created
void Scene::completeSetup()
{
	if (!_ready)
	{
		// Create the BVH tree if not already created
		_bvh.createTree();
		_ready = true;
	}
}

// Test if the ray intersects any object on its way to the light
bool Scene::shadowTest(Ray& ray, double t_max) const
{
	double t;
	Point3d p;
	Vector3d n;
	Color3d texColorTmp(_background);
	Object *obj;

	// Intersect the ray with any object (not necessarily nearest)
	if (_bvh.findObject(ray, t_max, &obj, t, p, n, texColorTmp, false))
	{
		return true;
	}

	return false;
}

// traceConeRays - Multiple ray-scene intersection after ray perturbation
Color3d Scene::traceConeRays(Ray ray, double vis, double eta, size_t numRays) const
{
	if (numRays <= 1)
	{
		// Trace a single ray
		return traceRay(ray, vis, eta);
	}

	// Sum different perturbated rays
	Color3d result(0.0f);
	for (size_t i = 0; i < numRays; i++)
	{
		result += traceRay(perturbateRay(ray), vis, eta);
	}

	// Average the results
	return result /= numRays;
}

// traceRay - Ray scene intersection for color rays
Color3d Scene::traceRay(Ray ray, double vis, double eta) const
{
	if (vis <= 0.1)
	{
		return _background;
	}
	else
	{
		Object *nearestObj = NULL;
		double nearestT;
		Point3d nearestP;
		Vector3d nearestN;
		Color3d nearestTexColor;

		nearestTexColor = Color3d(1, 1, 1);

		if (findNearestObject(ray, &nearestObj, nearestT, nearestP, nearestN, nearestTexColor))
		{
			Color3d refl(0.0), refra(0.0);
			bool reflected = false;

			// Check if the object is transparent
			if (nearestObj->getTransparency().max() > EPS)
			{
				// Calculate refraction
				refra = calcRefraction(ray, nearestP, nearestN, *nearestObj, reflected, vis, eta);
			}

			// Check if the object is reflective (and not already reflected due to total internal reflection)
			if (!reflected && nearestObj->getReflection().max() > EPS)
			{
				// Calculate reflection
				refl = calcReflection(ray, nearestP, nearestN, *nearestObj, vis, eta);
			}

			Color3d direct(0.0), intensity(0.0);

			// Add the ambient light
			direct += _ambientLight._color * nearestObj->getAmbient();

			// Test point light shadows and sum lights
			for (unsigned int i = 0; i < _lights.size(); i++)
			{
				Vector3d toLight = _lights[i]->_position - nearestP;
				Ray shadowRay = Ray(nearestP, toLight, ray.getTime());

				if (!shadowTest(shadowRay, toLight.norm()))
				{
					direct += phong(ray, *(_lights[i]), *nearestObj, nearestP, nearestN, nearestTexColor);
				}
			}

			// Test sphere light shadows and and sum lights
			for (unsigned int i = 0; i < _sphereLights.size(); i++)
			{
				Vector3d toLight = _sphereLights[i]->getCenter() - nearestP;
				Ray shadowRay = Ray(nearestP, toLight, ray.getTime());

				// Calculate the cone's cutoff angle to cover the sphere's diameter
				double cutoffAngle = atan2(_sphereLights[i]->getRadius(), toLight.norm());
				int softShadow = 0;
				Color3d shadowTex;
				Point3d shadowPoint;
				Vector3d shadowNormal;
				double tInf = std::numeric_limits<double>::infinity();
				double shadowT;

				// Check if the object is the light source itself
				if (nearestObj->asLightSource() == static_cast<const Light *>(_sphereLights[i]))
				{
					// If the object is the light source, use the light color
					direct += nearestTexColor * _sphereLights[i]->getColor();
				}
				else
				{
					// Else, calculate the lighting

					// Count the percentage of rays that intersect
					for (int j = 0; j < NUM_SHADOWS; j++)
					{
						Ray r = perturbateRay(shadowRay, cutoffAngle);

						if (!_sphereLights[i]->intersect(r, tInf, shadowT, shadowPoint, shadowNormal, shadowTex))
						{
							continue;
						}

						softShadow += 1 - (int)shadowTest(r, shadowT - EPS);
					}

					// Calculate phong shading for the light if not shadowed
					if (softShadow > 0)
					{
						direct += phong(ray, *(_sphereLights[i]), *nearestObj, nearestP, nearestN, nearestTexColor) * ((double)softShadow / (double)NUM_SHADOWS);
					}
				}
			}

			intensity = direct + refl + refra;

			// Clamp the intensity (RGB) to [0, 1]
			intensity[0] = (intensity[0] < 0) ? 0 : ((intensity[0] > 1) ? 1 : intensity[0]);
			intensity[1] = (intensity[1] < 0) ? 0 : ((intensity[1] > 1) ? 1 : intensity[1]);
			intensity[2] = (intensity[2] < 0) ? 0 : ((intensity[2] > 1) ? 1 : intensity[2]);

			return intensity;
		}
		else
		{
			// No intersection, return background color
			return _background;
		}
	}
}

Color3d Scene::phong(const Ray &ray, const Light &light, const Object &object, const Vector3d &P, const Vector3d& N, const Color3d& texColor) const
{
	Vector3d lightVec = (light.getPosition() - P).normalized();

	double reflectedDiffuse = dot(N, lightVec);
	Vector3d reflectedIdeal = 2.0 * N * (reflectedDiffuse) - lightVec;

	reflectedDiffuse = reflectedDiffuse < 0 ? 0 : reflectedDiffuse;

	Vector3d viewer = (ray.O() - P).normalized();
	Vector3d diffuse = light.getColor() * reflectedDiffuse;
	double reflectedProj = dot(reflectedIdeal, viewer);
	Vector3d specular = light.getColor() * pow(reflectedProj < 0 ? 0 : reflectedProj, object.getShining());
	
	return texColor * object.getDiffuse() * diffuse + object.getSpecular() * specular;
}

// add_object - add the given object to the scene
void Scene::add_object(Object * obj)
{
	_bvh.addObject(obj);
}

// add_light - add the given light to the scene
void Scene::add_light(PointLight * light)
{
	_lights.push_back(light);
}

// add_light - add the given light to the scene
void Scene::add_light(SphereLight * light)
{
	_sphereLights.push_back(light);
	add_object(light);
}

// Returns a ray in a cone with cutoff _cutoff around the input ray r
Ray Scene::perturbateRay(const Ray& r) const
{
	return perturbateRay(r, _cutoffAngle);
}

// Returns a ray in a cone with cutoff cutoffAngle around the input ray r
Ray Scene::perturbateRay(const Ray& r, double cutoffAngle) const
{
	double phi = ((double)rand() / (double)RAND_MAX) * cutoffAngle;
	double theta = ((double)rand() / (double)RAND_MAX) * 2 * PI;

	return perturbateRay(r, phi, theta);
}

// Returns a ray perturbated by phi and theta
Ray Scene::perturbateRay(const Ray& r, double phi, double theta) const
{
	double cx = cos(theta) * sin(phi);
	double cy = sin(theta) * sin(phi);
	double cz = cos(phi);
	
	Vector3d w = r.D(); // Get the new Z axis (D points in the Z direction of the spherical coordinate system)
	Vector3d u = Vector3d(0, 1, 0) % w; // Get the new X axis

	if (u.length() < EPS)
	{
		// Handle rays pointing to (0, 1, 0)
		u = Vector3d(1, 0, 0) % w;
	}

	Vector3d v = w % u; // Get the new Y axis
	
	// Project to the standard coordinate system
	double px = u[0] * cx + v[0] * cy + w[0] * cz;
	double py = u[1] * cx + v[1] * cy + w[1] * cz;
	double pz = u[2] * cx + v[2] * cy + w[2] * cz;

	return Ray(r.O(), Vector3d(px, py, pz), r.getTime());
}

Scene::~Scene() {

}

bool Scene::findNearestObject(IN Ray ray, OUT Object** object, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const
{
	// Intersect the ray with the BVH to find the nearest object
	return _bvh.findObject(ray, std::numeric_limits<double>::infinity(), object, t, P, N, texColor, true);
}

// calculate the reflection color at the given intersection point
Color3d Scene::calcReflection(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, double vis, double eta, bool isCritical) const
{
	Vector3d I = ray.D();
	Vector3d r1 = I - 2 * (N | I) * N;
	Ray rNew = Ray(P, r1, ray.getTime());
	Color3d color = isCritical ? Color3d(1, 1, 1) : object.getReflection();
	return color * traceRay(rNew, vis/2, eta);
}

// calculate the refraction color at the given intersection point
Color3d Scene::calcRefraction(const Ray& ray, const Point3d& P, const Vector3d& N, const Object& object, bool &reflected, double vis, double etaI) const
{
	Vector3d I = -ray.D().normalized();
	double etaT = object.getIndex();
	Vector3d N2 = N.normalized();
	double projI = (N2 | I);

	if (projI < 0)
	{
		N2 *= -1;
		projI *= -1;
		etaT = 1;
		etaI = object.getIndex();
	}

	double eta = etaI / etaT;
	double sq = 1 - (eta * eta) * (1 - projI * projI);

	reflected = (sq < 0);

	if (reflected)
	{
		// Total internal reflection
		return calcReflection(ray, P, N2, object, vis, etaI, true);
	}

	Vector3d t = (eta * projI - sqrt(sq)) * N2 - (eta * I);
	Ray rNew = Ray(P, t, ray.getTime());
	return object.getTransparency() * traceRay(rNew, vis / 2, etaT);
}
