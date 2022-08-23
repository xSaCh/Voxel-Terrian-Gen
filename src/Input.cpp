#include <iostream>
#include "Input.h"

Input *Input::instance = 0;

Input::Input()
{
    mousePos = vec2(0.0f);
}

void Input::Init(GLFWwindow *window)
{
    Input *i = getInstance();
    i->win = window;
    i->hasInit = true;
    glfwSetKeyCallback(i->win, keyCallback);

    glfwSetMouseButtonCallback(i->win, mouseCallback);
    glfwSetCursorPosCallback(i->win, mousePosCallback);
}

void Input::Update()
{
    for (auto &i : keys)
        i.second = false;

    for (auto &i : mouseKeys)
        i.second = false;

    // if (drag)
    //     glfwSetWindowPos(win, wrel_cp_x - cp_x, wrel_cp_y - cp_y);
}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Input *inst = Input::getInstance();
    if (!inst->hasInit)
        return;
    if (action != GLFW_REPEAT)
        inst->keys[key] = action;
}

void Input::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    Input *inst = Input::getInstance();
    if (!inst->hasInit)
        return;
    if (action != GLFW_REPEAT)
        inst->mouseKeys[button] = action;

    // if ((glfwGetKey(inst->win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS))
    // {
    //     inst->drag = true;
    //     double x, y;
    //     glfwGetCursorPos(inst->win, &x, &y);
    //     glfwGetWindowPos(inst->win, &(inst->wX), &(inst->wY));

    //     inst->mX = inst->wX + (int)x;
    //     inst->mY = inst->wY + (int)y;
    // }
    // if ((glfwGetKey(inst->win, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) || (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE))
    // {
    //     inst->drag = false;
    // }
}

void Input::mousePosCallback(GLFWwindow *window, double mouseXpos, double mouseYpos)
{
    Input *inst = Input::getInstance();
    if (!inst->hasInit)
        return;

    if (inst->drag)
    {
        int winPosX, winPosY;
        glfwGetWindowPos(inst->win, &winPosX, &winPosY);

        mouseXpos += winPosX;
        mouseYpos += winPosY;

        int fX = inst->wX + ((int)mouseXpos - inst->mX);
        int fY = inst->wY + ((int)mouseYpos - inst->mY);

        glfwSetWindowPos(inst->win, fX, fY);
    }
}

vec2 Input::GetMousePos()
{
    double mx, my;
    glfwGetCursorPos(win, &mx, &my);
    mousePos = vec2((float)mx, (float)my);
    return mousePos;
}

bool Input::getMouse(int glfwMouseButtonCode)
{
    int s = glfwGetMouseButton(win, glfwMouseButtonCode);

    return s == GLFW_PRESS;
}

bool Input::isKeyDown(int glfwKeyCode)
{
    if (!keys.count(glfwKeyCode))
        return false;

    return keys[glfwKeyCode];
}

bool Input::isMouseButtonDown(int glfwMouseButtonCode)
{
    if (!mouseKeys.count(glfwMouseButtonCode))
        return false;

    return mouseKeys[glfwMouseButtonCode];
}