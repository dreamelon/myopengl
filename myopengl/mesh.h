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
    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

public:
    void Draw();

protected:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    void setupMesh();

};

class Quad : public Mesh {
public:
    Quad() {
        vertices = {
            { -1.f,  1.f, 0.f, 0, 1, 0, 0.f, 1.f },
            { -1.f, -1.f, 0.f, 0, 1, 0, 0.f, 0.f },
            {  1.f,  1.f, 0.f, 0, 1, 0, 1.f, 1.f },
            {  1.f, -1.f, 0.f, 0, 1, 0, 1.f, 0.f },
        };
        indices = { 0, 1, 2, 1, 3, 4 };
        setupMesh();
    }
};

class Sphere : public Mesh {
public:
    Sphere() {
        const unsigned int X_SEGMENTS = 128;
        const unsigned int Y_SEGMENTS = 128;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                vertices.push_back({ xPos, yPos, zPos, xPos, yPos, zPos, xSegment, ySegment });
                //positions.push_back(glm::vec3(xPos, yPos, zPos));
                //uv.push_back(glm::vec2(xSegment, ySegment));
                //normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        setupMesh();
    }
};

class Cube : public Mesh {
public:
    Cube() {
        vertices = {
            // Back face
            { -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f }, // Bottom-left
            {  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f }, // top-right
            {  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f }, // bottom-right
            { -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f }, // top-left

            // Front face
            { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f }, // bottom-left
            {  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f }, // bottom-right
            {  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f }, // top-right
            { -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f }, // top-left

            // Left face
            { -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f }, // top-right
            { -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f }, // top-left
            { -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f }, // bottom-left
            { -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f }, // bottom-right


            // Right face
            {  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f }, // top-left
            {  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f }, // bottom-right
            {  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f }, // top-right
            {  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f }, // bottom-left

            // Bottom face
            { -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f }, // top-right
            {  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f }, // top-left
            {  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f }, // bottom-left
            { -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f }, // bottom-right

            // Top face                             
            { -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f }, // top-left
            {  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f }, // bottom-right
            {  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f }, // top-right
            { -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f }  // bottom-left
        };

        indices = {
            0, 1, 2,
            1, 0, 3,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            13, 12, 15,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            21, 20, 23
        };

        setupMesh();
    }
    
};