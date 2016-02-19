Description:
-----------
Drawing a circle:

I draw a circle by dividing it to triangles, I change the number of triangles according to the window size (based on
a threshold value - if the circle is small enough, I can decrease the number of triangles).
When the number of the triangles is changed I send to OpenGL a different buffer of vertices.

Making it bigger/smaller:
There is no need to send a new vertex array each time the window size is changing - it is enough to scale the existing
vertices based on the window size. Thus I pass the size of the window to the vertex shader and scale the vertices there.
I also keep the aspect ratio, keeping the circle a circle rather than an ellipse even if the window size is not square.

Texture:
Done by toggle the pixels' color for groups of square pixels (modulo operation on X and Y coordinates).
