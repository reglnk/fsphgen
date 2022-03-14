# Sphere Generator
UV sphere model generator for OpenGL 3.x apps.
Output is C file with vertex array (VBO data, floats) and index array (EBO data, ints).
Each vertex contains 5 floats: coordinates of point on sphere surface and coords of texture next to them.
Each unsigned int of Index array is a pointer to vertex.
Each index string contains one triangle in first and last sphere layer, 2 triangles (that are 1 rectangle) in middle layers.

Usage example:

static void initSphGraphics()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphVerts), sphVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphIndices), sphIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // 

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
