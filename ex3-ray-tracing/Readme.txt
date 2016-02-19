See https://docs.google.com/document/d/1KhKAX7RIXXf3kPmTshdAIOaPpeNS1Rj4RckAjkTTCEo/edit?usp=sharing 

Bounding Volume Hierarchy (BVH):
-------------------------------
Implemented by sorting bounding boxed by Z and constructing a binary tree.
BVH is used in MyMeshObject (for sorting polygon objects) and in the scene class (for sorting scene objects).
Because of the BVH, the scene must be notified that it is ready to construct the BVH. This is done by adding
a call to Scene::completeSetup() at the end of setupScene() in SceneChooser.h.


Spherical lights:
-----------------
Implemented by inheriting both Light abstract base class and the Sphere object class.
When testing shadows random rays are shot towards the sphere (using perturbateRay).
I iterate over the spherical lights and I shoot rays to the light. I check which ray reaches the light (like point lights).
I calculate color = color * (number_of_hits / number_of_rays).


Custom objects:
--------------
- Spherical cone (bottom is spherical, not flat) - without texture mapping.
- Open cylinder (without caps) - with texture mapping.
- Ellipsoid: Implemented by transforming into a sphere located at the origin.


Custom scenes:
-------------
Implemented 2 additional scenes.
- Scene #3 (MyScene1.h): "BB-8 on the run": BB-8 is the Star Wars' new star [Notice the double meaning :-)]. It Demonstrates motion blur. Uses the spherical cone custom object.
                          See BB-8 on action! https://www.youtube.com/watch?v=yoeivPyKxh8

- Scene #4 (MyScene2.h): "The astronomer's room": Includes the ellipsoid, spherical cone and cylinder, with spherical light sources (top and desk light).

To render the scenes use the regular executable (ex3) with the relevant scene number parameter.



Bonus:
-----
- Depth of Field: Implemented by rendering each pixel from different camera positions within the camera lens' diameter, then averaging the pixel's color.
                  To render using depth of field, make using "make clean" and then "make DOF=1" and execute "ex3".
                  (To return to normal mode, "make clean" and then "make" and execute "ex3").
                  See examples: with dof: scene-2-with-dof.bmp, without dof: scene-2-without-dof.bmp.

- Motion blur:    Each scene is assigned a maximum time value (defaults to 1). I implemented a dynamic object, which holds a different object for each time
                  and renderes the relevant object at each time.
                  Each pixel is rendered multiple times, once for each scene time (each ray is assigned a time value), and the colors are averaged.
                  To simulate motion the moving objects are constructed of different objects with different parameters at each time (for example different position at each time).
                  Scene #3 (MyScene1.h) demonstrated the Motion Blur effect (the effect depends only on the scene, not on the executable).
                  See examples: with motion blur: scene-3-with-motion-blur.bmp, without motion blur: scene-3-without-motion-blur.bmp.
                  (to disable motion blur, the scene's total time was changed to 1 in MyScene1.h).




Speed and quality measures:
--------------------------
Ran on aquarium computer "hup-32". DOF is disabled.

- Scene #0: "ex3 2 2 3 0". The strings are more clear, the specular is more shiny considering rendering time.

- Scene #1: "ex3 1 6 6 1". The reflection look more realistic when I increase number of reflection rays.
            The shininess 'lasts'

- Scene #2: "ex3 1 4 130 2". When I increase the first parameter the render time increases badly. Rendering mesh take a lot of time.

- Scene #3: "ex3 1 4 30 3". When I increase the number of rays per pixel I see less shadow patterns (aliasing) on the head area. I used more
            reflection rays (4) to emphasize the specularity on the eye.

- Scene #4: "ex3 1 1 0 4". This scene contains a lot of objects, and it is really detailed. Therefore, I choose the quickest parameters choice.

Total time for the first third scenes:
time ex3 1 1 0 0: 2.214u 0.012s 0:02.27 97.7%	0+0k 360+1472io 0pf+0w
time ex3 1 1 0 1: 1.813u 0.017s 0:01.85 98.3%	0+0k 0+1472io 0pf+0w
time ex3 1 1 0 2: 3.734u 0.014s 0:03.77 99.2%	0+0k 0+1480io 0pf+0w

Total time for the first third scenes(with DOF: make DOF=1):
time ex3 1 1 0 0: 22.133u 0.094s 0:22.70 97.8%	0+0k 744+1472io 1pf+0w
time ex3 1 1 0 1: 18.387u 0.014s 0:18.43 99.7%	0+0k 0+1480io 0pf+0w
time ex3 1 1 0 2: 36.709u 0.159s 0:36.91 99.8%	0+0k 0+1480io 0pf+0w



Websites:
--------
http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction
http://mathworld.wolfram.com/SphericalCoordinates.html
http://mathworld.wolfram.com/Cone.html
