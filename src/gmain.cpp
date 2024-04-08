#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"
#include "gfx/texture.h"
#include "Camera.h"

enum class BlockFace
{
    Top,
    Bottom,
    Front,
    Back,
    Right,
    Left
};

struct GMesh
{
    std::vector<float> vertices, normals, texcoords;

    void update(const GMesh &other, std::size_t x, std::size_t y, std::size_t z)
    {
        for (std::size_t i = 0; i < other.vertices.size() / 3; ++i)
        {
            vertices.push_back(other.vertices[i * 3] + x);
            vertices.push_back(other.vertices[1 + i * 3] + y);
            vertices.push_back(other.vertices[2 + i * 3] + z);
        }
        normals.insert(normals.end(), other.normals.begin(), other.normals.end());
        texcoords.insert(texcoords.end(), other.texcoords.begin(), other.texcoords.end());
    }

    std::size_t triangleCount()
    {
        return vertices.size() / 3;
    }
};


inline GMesh getMeshFace(BlockFace face)
{
    if (face == BlockFace::Top)
        return GMesh{
            {-0.5f, 0.5f, -0.5f,
             0.5f, 0.5f, -0.5f,
             0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f,
             -0.5f, 0.5f, 0.5f,
             -0.5f, 0.5f, -0.5f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
    else if (face == BlockFace::Bottom)
        return GMesh{
            {-0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, 0.5f,
             0.5f, -0.5f, 0.5f,
             -0.5f, -0.5f, 0.5f,
             -0.5f, -0.5f, -0.5f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
    else if (face == BlockFace::Front)
        return GMesh{
            {-0.5f, -0.5f, 0.5f,
             0.5f, -0.5f, 0.5f,
             0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f,
             -0.5f, 0.5f, 0.5f,
             -0.5f, -0.5f, 0.5f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
    else if (face == BlockFace::Back)
        return GMesh{
            {-0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, 0.5f, -0.5f,
             0.5f, 0.5f, -0.5f,
             -0.5f, 0.5f, -0.5f,
             -0.5f, -0.5f, -0.5f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
    else if (face == BlockFace::Right)
        return GMesh{
            {0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, 0.5f,
             0.5f, 0.5f, 0.5f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
    else // face == BlockFace::Left
        return GMesh{
            {-0.5f, 0.5f, 0.5f,
             -0.5f, 0.5f, -0.5f,
             -0.5f, -0.5f, -0.5f,
             -0.5f, -0.5f, -0.5f,
             -0.5f, -0.5f, 0.5f,
             -0.5f, 0.5f, 0.5f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 0.0f,
             1.0f, 1.0f,
             0.0f, 1.0f,
             0.0f, 0.0f}};
}

namespace Chunk
{
    std::size_t Width = 16, Height = 16, Depth = 16;
}

void puts(const std::string &s)
{
    std::cout << s << std::endl;
}

GLFWwindow *win = NULL;
Camera cam(vec3(0.0));

void mouse_callback(GLFWwindow *win, double xpos, double ypos)
{
    cam.processMouse(xpos, ypos);
}

int main(int argc, char **argv)
{
#pragma region GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(800, 600, "OPENGL_LEARN", NULL, NULL);

    if (win == NULL)
    {
        std::cout << "Cant Create GLFW Window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(win);
    gladLoadGL();
    glViewport(0, 0, 800, 600);

    // Callback for resize window
    glfwSetCursorPosCallback(win, mouse_callback);

    glfwSwapInterval(1);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable Aplha Channel
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion

    cam.win = win;
    cam.lastX = 800 / 2.0f;
    cam.lastY = 600 / 2.0f;
    cam.yawA = 3.0f;
    cam.pitchA = -40.0f;
    cam.front.z = 0.04f;

    glm::mat4 projection = glm::perspective(radians(70.0f), (float)800 / (float)600, 0.1f, 100.0f);

    bool blocks[16][16][16];
    // "map generation"
    for (int x = 0; x < Chunk::Width; ++x)
    {
        for (int y = 0; y < Chunk::Height; ++y)
        {
            for (int z = 0; z < Chunk::Depth; ++z)
            {
                // putting a block everywhere (nearly)

                if (y == 10)
                    blocks[x][y][z] = false;
                else if (z > 12)
                    blocks[x][y][z] = false;
                else
                    blocks[x][y][z] = true;
            }
        }
    }

    // creating mesh
    GMesh temp_mesh;
    for (int x = 0; x < Chunk::Width; ++x)
    {
        for (int y = 0; y < Chunk::Height; ++y)
        {
            for (int z = 0; z < Chunk::Depth; ++z)
            {
                if (x > 0 && blocks[x - 1][y][z] == false)
                {
                    // add face back
                    temp_mesh.update(getMeshFace(BlockFace::Back), x - 1, y, z);
                }
                if (y > 0 && blocks[x][y - 1][z] == false)
                {
                    // add face bottom
                    temp_mesh.update(getMeshFace(BlockFace::Bottom), x, y - 1, z);
                }
                if (z > 0 && blocks[x][y][z - 1] == false)
                {
                    // add face left
                    temp_mesh.update(getMeshFace(BlockFace::Left), x, y, z - 1);
                }
                if (x + 1 < Chunk::Width && blocks[x + 1][y][z] == false)
                {
                    // add face front
                    temp_mesh.update(getMeshFace(BlockFace::Front), x + 1, y, z);
                }
                if (y + 1 < Chunk::Height && blocks[x][y + 1][z] == false)
                {
                    // add face top
                    temp_mesh.update(getMeshFace(BlockFace::Top), x, y + 1, z);
                }
                if (z + 1 < Chunk::Depth && blocks[x][y][z + 1] == false)
                {
                    // add face right
                    temp_mesh.update(getMeshFace(BlockFace::Right), x, y, z + 1);
                }
            }
        }
    }

    std::cout << temp_mesh.vertices.size() << " vertices" << std::endl;

    // creating vertices
    std::vector<float> temp_vertices;
    for (std::size_t i = 0; i < temp_mesh.vertices.size() / 3; ++i) // we will move from one line to another
    {
        temp_vertices.push_back(temp_mesh.vertices[i * 3]);
        temp_vertices.push_back(temp_mesh.vertices[1 + i * 3]);
        temp_vertices.push_back(temp_mesh.vertices[2 + i * 3]);

        temp_vertices.push_back(temp_mesh.texcoords[i * 3]);
        temp_vertices.push_back(temp_mesh.texcoords[1 + i * 3]);
    }

    VAO vao;
    vao.bind();
    VBO vbo(&(temp_vertices[0]), temp_vertices.size() * sizeof(float));
    vao.linkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    vao.linkVBO(vbo, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));

    Shader shader("./res/git.vert", "./res/git.frag");

    shader.activate();
    Texture tex("./res/brick.png");
    shader.setMat4("projection", value_ptr(projection));
    glm::mat4 view = glm::mat4(1.0f);
    view = cam.getViewMatrix();
    shader.setMat4("view", value_ptr(view));

    float prevtime = 0;
    float deltaTime = 0;
    bool isWire;
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(win))
    {

        glfwPollEvents();
        cam.update(deltaTime);

        glClearColor(0.49f, 0.67f, 0.95f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.activate();

        // render container
        vao.bind();
        tex.bind();
        // create transformations
        glm::mat4 model = glm::mat4(1.0f);
        // pass it to the shader
        view = cam.getViewMatrix();
        shader.setMat4("view", value_ptr(view));
        shader.setMat4("model", value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, temp_mesh.triangleCount());

        if ((glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS))
        {
            isWire = !isWire;
            printf("Cam:- X:%.2f Y:%.2f z:%0.2f\n", cam.pos.x, cam.pos.y, cam.pos.z);
            printf("Dir:- X:%.2f Y:%.2f z:%0.2f\n", cam.yawA, cam.pitchA, cam.front.z);

            // glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
            // c += 0.1f;
        }

        float curTime = glfwGetTime();
        deltaTime = curTime - prevtime;
        prevtime = curTime;

        // swap buffers
        glfwSwapBuffers(win);
    }

    return 0;
}
