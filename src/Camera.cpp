#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(vec3 pos)
{
	this->pos = pos;

	right = vec3(0.0f);
	up = vec3(0.0f);

	front = vec3(0, 0, -1.0f);
	worldUp = vec3(0, 1.0f, 0);

	yawA = 0.f;
	pitchA = 0.0f;

	cameraSpeed = 5.0f;
	sensitvity = 0.3f;
}

void Camera::update(float deltaTime)
{
	processInput(deltaTime);
}

void Camera::processMouse(float xPos, float yPos)
{
	float xOff = xPos - lastX;
	float yOff = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	xOff *= sensitvity;
	yOff *= sensitvity;

	yawA += xOff;
	pitchA += yOff;

	pitchA = clamp(pitchA, -89.0f, 89.0f);

	if (yawA >= 360)
		yawA -= 360;
	else if (yawA <= -360)
		yawA += 360;

	vec3 direction;
	direction.x = cos(radians(yawA)) * cos(radians(pitchA));
	direction.y = sin(radians(pitchA));
	direction.z = sin(radians(yawA)) * cos(radians(pitchA));

	front = normalize(direction);

	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
}

mat4 Camera::getViewMatrix()
{
	return lookAt(pos, pos + front, up);
	// mat4 trans = glm::translate(glm::mat4(1.0f), pos);

	// return inverse(trans);
}

void Camera::processInput(float deltaTime)
{
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = 7.0f;
	else if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cameraSpeed = 5.0f;

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		pos += cameraSpeed * deltaTime * front;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		pos -= cameraSpeed * deltaTime * front;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		pos += normalize(cross(front, up)) * deltaTime * cameraSpeed;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		pos -= normalize(cross(front, up)) * deltaTime * cameraSpeed;

	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos += cameraSpeed * deltaTime * up;
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		pos -= cameraSpeed * deltaTime * up;

	if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS)
		printf("PX:%f PY:%f P:%f Y:%f\n", pos.x, pos.y, pitchA, yawA);
}