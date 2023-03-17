#ifndef MESH_H
#define MESH_H

#include "Shader.hpp"
#include <vector>

struct Vertex {
    vec3 Position = { 0.0f, 0.0f, 0.0f };
    vec3 Normal = { 0.0f, 0.0f, 0.0f };
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    ~Mesh() {};

    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO;
    void initMesh();
};
#endif
