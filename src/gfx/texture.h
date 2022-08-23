#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <cstdint>

// Vertex Array object Class
class Texture
{
public:
    uint32_t ID;
    int width, height, channels;

    Texture(const char *filePath);
    Texture(uint32_t id) {ID = id;width=-1;height=-1;channels=-1;}
    void bind();
    void unbind();
    void deleteTexture();

private:
    uint8_t *rawPixeldata(const char *filePath);
};

#endif
