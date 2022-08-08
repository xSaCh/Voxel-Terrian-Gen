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

    void bind();
    void unbind();
    void deleteTexture();

private:
    uint8_t *rawPixeldata(const char *filePath);
};

#endif
