#include <iostream>
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *filePath)
{
    uint8_t *data = rawPixeldata(filePath);
    if (data == NULL)
    {
        std::cout << "[Error] Failed to load File " << filePath << '\n';
        return;
    }

    glGenTextures(1, &ID);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float col[] = {1, 0, 0, 1};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    unbind();

    stbi_image_free(data);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture()
{
    glDeleteVertexArrays(1, &ID);
}

uint8_t *Texture::rawPixeldata(const char *filePath)
{
    stbi_set_flip_vertically_on_load(true);
    uint8_t *data = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
    return data;
}