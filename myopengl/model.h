#pragma once
#include <vector>
#include "shader.h"
#include "mesh.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "stb_image.h"

class Model {
public:
    Model(const char *path) {
        loadModel(path);
    }
    Model() = default;
    void Draw();

private:
    std::vector<Mesh> meshes;

    void loadModel(std::string const path);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void processNode(aiNode* node, const aiScene* scene);
};
