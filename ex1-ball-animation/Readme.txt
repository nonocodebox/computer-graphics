Description:
-----------
Create moving colorful balls which shrink whenever they collide with each other or the wall.
I used the ball model I created in part A and changed the fragment shader, because I now
want a uniform color for the balls with extra highlight. I have also added a sub class 
called "Ball Properties" in order to keep information such as radius, ball direction and 
cumulative transformation matrix. I also changed the function timer() in ex0.cpp to animation mode.
I passed the ball vertices only once to the vertex shader and created an array of different 
transformtion matrices in order to draw the same ball in different places each time.
In the host program, I saved for each ball a matrix that represents its current location 
after any use of transformation on it. That is, I multiplied this matrix with the transformation 
matrix every time I moved the ball. 
In order to draw a ball wherever the mouse was pressed, I had to translate the mouse coordinate 
system into the screen coordinate system by some transformations.
I implemented the highlights on the ball such that the light gradient decrease linearly from the 
center of lighting. I chose the center of lighting by calculating the direction from the light 
source to the center of the ball, and placing the lighting source at 0.5 * Radius. I also added
constant ambient light to make the balls more bright irrespective of the light.

Bonus:
------------
I have added 1 more light, which is calculated exactly the same way the first light was calculated.
The second light has a different source location. The total light is calculated by 
0.2*Ambient + 0.5*Light1 + 0.5*Light2 - This means the ball could be completely highlighted even
with only partial lighting, depending on the position of the two lights (this looks more realistic).

I have also added movement of the light sources. Each step, every light source is multiplied by a
rotation matrix, rotating around the Z axis by a fixed amount. This makes the light sources circle 
the scene constantly.

I have implemented antialiasing by setting the source blending mode to GL_SRC_ALPHA and destination
blending mode to GL_ONE_MINUS_SRC_ALPHA. This results in the alpha value controlling the blending ratio
between the source and destination colors.
I have calculated the alpha value by using a smooth step function on the border pixels of the circle,
using smoothstep() and fwidth() as explained in the first URL.
