# Sphere Generator
UV sphere model generator for OpenGL 3.x apps.
Output is C file with vertex array (VBO data, floats) and index array (EBO data, ints).
Each vertex contains 5 floats: coordinates of point on sphere surface and coords of texture next to them.
Each unsigned int of Index array is a pointer to vertex.
Each index string contains one triangle in first and last sphere layer, or 2 triangles (that are 1 rectangle) in middle layers.

# Example of generated sphere usage:
```c
// Init
{
    glGenVertexArrays(1, &sphVAO);
    glGenBuffers(1, &sphVBO);
    glGenBuffers(1, &sphEBO);

    glBindVertexArray(sphVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphVerts), sphVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphIndices), sphIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
// In rendering loop
{
    glUseProgram(glShaderProgID);
    glUniform1i(glGetUniformLocation(glShaderProgID, "sph_tex"), num);
    glUniformMatrix4fv(glGetUniformLocation(glShaderProgID, "view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(glShaderProgID, "projection"), 1, GL_FALSE, &projMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(glShaderProgID, "transform"), 1, GL_FALSE, &transformMat[0][0]);

    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(GL_TEXTURE0 + num, tex_sphere_0);

    glBindVertexArray(sphVAO);
    glDrawElements(GL_TRIANGLES, sizeof(sphIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}
```

# Shader example:
Vertex
```glsl
#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 wTexCoords;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * transform * vec4(vPosition, 1.f);
    wTexCoords = vec2(vTexCoords);
}
```

Fragment
```glsl
#version 330 core

in vec2 wTexCoords;
out vec4 FragColor;

uniform sampler2D sph_tex;

void main() {
    FragColor = texture(sph_tex, wTexCoords);
}
```
