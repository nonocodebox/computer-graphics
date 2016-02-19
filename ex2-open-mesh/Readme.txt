Description:
------------

I started from exercise 2b code. I started by changing Phong vertex and fragment shader.
Then, adding features one by one: starting from wood texture, marble, mirror and brick.
Finally I added the GUI supporting pressing buttons etc.

In the VS, I created the texture coordinates in 3D (texturePosition), then the
ks,kd in each pixel are calulated according to the texture type.

When changing between texture modes the parameters are reset to the texture's defaults (turbulance
scale and texture scale). Also when pressing 'R' these parameters are reset (along with rotation,
translation and scale).

Glow:

I have read about the method here:
http://forum.devmaster.net/t/shader-effects-glow-and-bloom/3100
http://www.gamasutra.com/view/feature/130520/realtime_glow.php?page=2
(These are the two links from the TA slides).

Rendering is done in multiple steps:
1. Render the regular scene to a framebuffer (using the current mode's shader - Phong/Gouraud/RGB).
2. Attenutate dark colors and blur the result horizontally using a 1D gaussian into another
   framebuffer (using the Gaussian shader).
3. Blur the result vertically (1D gaussian again) into another framebuffer (using the Gaussian shader
   again).
4. Blend the two framebuffers (original model and blurred) into the screen's framebuffer (using the
   Glow shader).

To render the contents of a framebuffer (and manipulate it using another shader) I use a simple
object - A plane from [-1, -1] to [1, 1] using two triangles. The framebuffer's texture is mapped to
this plane and the plane is rendered using filled triangles mode.
This means that the actual model (model vertices with lighting calculations) is rendered only once!

NOTE 1: The gaussian kernel's size is also reset when pressing 'R'.
NOTE 2: Glow is disabled in wireframe mode, enabling glow disables wireframe mode.
