# fsphgen
UV sphere model generator for OpenGL apps.
Output is C header file containing vertices array and indices array.
Vertices are to be passed to VBO, indices respectively to EBO.
Each vertice contains 5 floats: coordinates of point on sphere surface and coords of texture next to them.
One regular line of Indices array contains 2 triangles that make up a rectangle.
Index lines of first or last layer of sphere contain one triangle, so first vertice and last vertice are not used.
