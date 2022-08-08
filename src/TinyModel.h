#ifndef TINYMODEL_H
#define TINYMODEL_H

#include <iostream>
#include <vector>

#include "tiny_obj_loader.h"
#include "gfx\mesh.h"

class TinyModel
{
public:
    TinyModel(string path);
    // {
    //     loadModel(path);
    // }
    void draw(Shader &shader);

private:
    // model data
    vector<Mesh *> meshes;
    string directory;
    vector<TextureInfo> textures_loaded;

    void loadModel(string path);
    // void processNode(aiNode *node, const aiScene *scene);
    // Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    // vector<TextureInfo> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
    //                                          string typeName);
};

#endif