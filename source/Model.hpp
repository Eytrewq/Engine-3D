#ifndef MODEL_H
#define MODEL_H

#include "Mesh.hpp"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    const aiScene* scene;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string const& path);
    ~Model() {};

    void Draw(Shader& shader);
private:
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif
