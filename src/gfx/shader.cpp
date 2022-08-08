#include <iostream>
#include <fstream>
#include "shader.h"

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    // Genereating Shader objs
    uint32_t vertShad = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragShad = glCreateShader(GL_FRAGMENT_SHADER);

    // Get Shader SourceCode
    const char *vs = getShaderFileData(vertexFile);
    const char *fs = getShaderFileData(fragmentFile);

    // Compiling ShaderSources
    glShaderSource(vertShad, 1, &vs, NULL);
    glCompileShader(vertShad);
    getShaderError(vertShad, "Vertex");

    glShaderSource(fragShad, 1, &fs, NULL);
    glCompileShader(fragShad);
    getShaderError(fragShad, "Fragment");

    // Attack Shaders to ShaderProgram
    ID = glCreateProgram();
    glAttachShader(ID, vertShad);
    glAttachShader(ID, fragShad);
    glLinkProgram(ID);

    glDeleteShader(vertShad);
    glDeleteShader(fragShad);
}

void Shader::activate()
{
    glUseProgram(ID);
}

void Shader::setInt(const char *uniformName, int value)
{
    glUniform1i(getUniformLocation(uniformName), value);
}

void Shader::setFloat(const char *uniformName, float value)
{
    glUniform1f(getUniformLocation(uniformName), value);
}
void Shader::setVec3(const char *uniformName, const glm::vec3 value)
{
    glUniform3f(getUniformLocation(uniformName), value.x, value.y, value.z);
}
void Shader::setVec4(const char *uniformName, const glm::vec4 value)
{
    glUniform4f(getUniformLocation(uniformName), value.w, value.x, value.y, value.z);
}

void Shader::setMat4(const char *uniformName, float *value_ptr)
{
    glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, value_ptr);
}

void Shader::deleteProgram()
{
    glDeleteProgram(ID);
}

Shader::~Shader()
{
    deleteProgram();
}

int Shader::getUniformLocation(const char *uniformName)
{
    int uniformId = glGetUniformLocation(ID, uniformName);
    return uniformId;
}

const char *Shader::getShaderFileData(const char *shaderfile)
{
    std::ifstream inpFile(shaderfile, std::ios::binary | std::ios::ate);

    int size = inpFile.tellg();
    inpFile.seekg(0, std::ios::beg);

    char *data = new char[size + 1];
    data[size] = '\0';

    inpFile.read(data, size);
    inpFile.close();
    return data;
}

void Shader::getShaderError(uint32_t shader, const char *file)
{
    int success;
    char info[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info);
        std::cout << "[" << file << " SHADER] " << info << '\n';
    }
}