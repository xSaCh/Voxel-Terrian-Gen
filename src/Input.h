#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

using namespace glm;
class Input
{
public:
    static Input *getInstance()
    {
        if (!instance)
            instance = new Input();
        return instance;
    }
    ~Input() {}
    void Init(GLFWwindow *window);
    void Update();
    vec2 GetMousePos();

    bool getMouse(int glfwMouseButtonCode);
    bool isKeyDown(int glfwKeyCode);
    bool isMouseButtonDown(int glfwMouseButtonCode);

private:
    static Input *instance;
    Input();

    std::unordered_map<int, bool> keys;
    std::unordered_map<int, bool> mouseKeys;

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void mousePosCallback(GLFWwindow *window, double mouseXpos, double mouseYpos);

    bool hasInit;
    GLFWwindow *win;

    vec2 mousePos;
    bool drag = false;
    int mX = 0, mY = 0;
    int wX = 0, wY = 0;
};

#endif