#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "gfx\mesh.h"

class Model
{
public:
    Model(string path)
    {
        loadModel(path);
        printf("MODEL\n");
        for (int i = 0; i < meshes.size(); i++)
        {
            // printf("[Debug] vao: %p vbo: %p ebo: %p\n", meshes[i].vao, meshes[i].vbo, meshes[i].ebo);
            // printf("[Debug] vao: %i %p vbo: %i %p ebo: %i %p\n", meshes[i].vao->ID, meshes[i].vao, meshes[i].vbo->ID, meshes[i].vbo, meshes[i].ebo->ID, meshes[i].ebo);
        }
    }
    void draw(Shader &shader);

private:
    // model data
    vector<Mesh *> meshes;
    string directory;
    vector<TextureInfo> textures_loaded;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    vector<TextureInfo> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             string typeName);
};

#endif