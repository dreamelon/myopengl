#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shader.h"
#include "texture.h"

struct Vertex
{
    Vertex() = default;
    Vertex(float x, float y, float z, float u, float v, float w, float s, float t) :
        Position(x, y, z),
        Normal(u, v, w),
        TexCoords(s, t)
    {}

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    Mesh():VAO(0), VBO(0), EBO(0){}
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

public:
    void Draw();
    void setupMesh();

protected:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

};

class Quad : public Mesh {
public:
    Quad();
};

class Sphere : public Mesh {
public:
    Sphere();
};

class Cube : public Mesh {
public:
    Cube();
};