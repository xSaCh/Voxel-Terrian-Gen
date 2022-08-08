#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <cstdint>

class Input
{
    void isKeyPress(int key);
    void getMousePos(int *xPos, int *Pos);
};

#endif