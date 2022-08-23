#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>

using namespace glm;

class Camera
{
public:
	Camera(vec3 pos);
	void update(float deltaTime);
	void processMouse(float xPos, float yPos);
	mat4 getViewMatrix();

	GLFWwindow *win;
	vec3 pos, front, right, up, worldUp;

	float yawA, pitchA;

	float cameraSpeed, sensitvity;

	float lastX, lastY;

private:
	void processInput(float deltaTime);

	//Camera rotation variables
};

#endif