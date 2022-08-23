#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cstdint>
#include <glm/glm.hpp>

class Shader
{
public:
    uint32_t ID;
    Shader(const char *vertexFile, const char *fragmentFile);
    void activate();
    void setInt(const char *uniformName, int value);
    void setFloat(const char *uniformName, float value);
    void setVec3(const char *uniformName, const glm::vec3 value);
    void setVec4(const char *uniformName, const glm::vec4 value);
    void setMat4(const char *uniformName, float *value_ptr);
    void deleteProgram();
    ~Shader();

private:
    const char *getShaderFileData(const char *shaderfile);
    void getShaderError(uint32_t shader, const char *file);
    int getUniformLocation(const char *uniformName);
};
#endif