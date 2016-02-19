/** For the students: DO NOT CHANGE THIS FILE */

#include <math.h>

#include "scene.h"
#include "sphere.h"
#include "polygon.h"
#include "ellipsoid.h"
#include "camera.h"
#include "MyMesh.h"
#include "MyMeshObject.h"

#include <stdio.h>

#include "SceneChooser.h"

#if __APPLE__
#include <dispatch/dispatch.h>
#include "profiling_block_timer.h"
#endif

int renderScene(size_t scene_index, size_t samplesPerPixel, size_t refRays, double cutoff)
{
	// Scene setup:
	Scene* scene = NULL;
	setupScene(&scene, scene_index, refRays, cutoff);

	if (scene == NULL)
	{
		std::cerr << "No scene" << std::endl;
		return - 1;
	}
	
	// Camera setup:
	Camera camera;
	scene->setDefaultCamera(camera);
	camera.setSamplesPerPixel(samplesPerPixel);
	
	// image parameters
	int w = 500;
	int h = 500;
#if __APPLE__
	__block
#endif
	BImage img(w,h);
	
	// Rendering:
	{
#if __APPLE__
		profiling_block_timer timer("Rendering time");
#endif
		
		// Render one row at a time, for easy future parallelization.
		// Assume that this loop can be parallelized in the future.
		// We use GCD on APPLE. There are some ported versions of libdispatch
		// (e.g. https://www.heily.com/trac/libdispatch and
		// https://github.com/DrPizza/libdispatch) but other parallelization APIs
		// such as TBB, might work better (I haven't checked this).
#if __APPLE__
		dispatch_apply(h, dispatch_get_global_queue(0, 0), ^(size_t i){
			camera.render(i, 1, img, *scene);
		});
#else
		for (size_t i = 0; i < h; i++)
		{
			// The render command:
			camera.render(i, 1, img, *scene);
		}
#endif
	}
	
	std::ostringstream filename;
	filename << "output_scene" << scene_index << ".bmp";
	
	img.save(filename.str().c_str());
	
	delete scene;
	
	return 0;
}


int main(int argc, const char* argv[])
{
	size_t samplesPerPixel = 1;
	if (argc > 1)
	{
		samplesPerPixel = atoi(argv[1]);
	}
	
	size_t refRays = 1;
	if (argc > 2)
	{
		refRays = std::max<size_t>(1,atof(argv[2]));
	}
	
	double cutoff = 0.0;
	if (argc > 3)
	{
		cutoff = std::max<double>(0.0, atof(argv[3]));
	}
	
	if (argc > 4) // Render selected scene
	{
		size_t scene_index = std::max(0,std::min(MAX_SCENE_INDEX, atoi(argv[4])));
		renderScene(scene_index, samplesPerPixel, refRays, cutoff);
	}
	else // Render all scenes
	{
		for (size_t scene_index = 0; scene_index <= MAX_SCENE_INDEX; scene_index++)
		{
			renderScene(scene_index, samplesPerPixel, refRays, cutoff);
		}
		
	}
}
