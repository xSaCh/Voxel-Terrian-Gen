#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyModel.h"
#include "gfx/stb_image.h"

uint32_t TextureFromFile(const string &path, const string &directory);

TinyModel::TinyModel(string path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./" + path.substr(0, path.find_last_of('\\'));
    ; // Path to material files
    reader_config.triangulate = true;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path, reader_config))
        if (!reader.Error().empty())
            std::cerr << "TinyObjReader: " << reader.Error();

    if (!reader.Warning().empty())
        std::cout << "TinyObjReader: " << reader.Warning();

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        vector<Vertex> vertices;
        vector<uint32_t> indices;
        vector<TextureInfo> textures;

        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                Vertex vert;

                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                vert.Position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                vert.Position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                vert.Position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    vert.Normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    vert.Normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    vert.Normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }
                else
                    std::cout << "No Normals\n";

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    vert.TexCoords.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    vert.TexCoords.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
                else
                    std::cout << "No TexCoords\n";

                indices.push_back(idx.vertex_index);
                vertices.push_back(vert);
                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;

            // per-face material
            int mat_id = shapes[s].mesh.material_ids[f];

            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), materials[mat_id].diffuse_texname.c_str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip && mat_id >= 0)
            {
                TextureInfo texture;
                texture.id = TextureFromFile(materials[mat_id].diffuse_texname, path.substr(0, path.find_last_of('\\')));
                texture.type = "texture_diffuse1";
                texture.path = materials[mat_id].diffuse_texname;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        // vector<TextureInfo> tex;
        // indices.clear();
        vector<uint32_t> del;
        Mesh *m = new Mesh(vertices, del, textures);
        meshes.push_back(m);
    }

    cout << "Total Mesh: " << meshes.size() << '\n';

    int c = 0;
    for (int i = 0; i < meshes.size(); i++)
        c += meshes[i]->vertices.size();
    cout << "Total vertices: " << c << '\n';
}

uint32_t TextureFromFile(const string &path, const string &directory)
{
    string filename = string(path);
    filename = directory + '\\' + filename;

    std::cout << "filename: " << filename << '\n';

    uint32_t textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << ":" << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void TinyModel::draw(Shader &shader)
{
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->draw(shader);
    }
}