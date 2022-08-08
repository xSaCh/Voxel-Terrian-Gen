// CPP program to render a triangle using Shaders
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
#include "gfx/mesh.h"
#include "Camera.h"
#include "model.h"
#include "Chunk/Chunk.h"

using namespace std;
using namespace glm;

#define WIDTH 800
#define HEIGHT 600

void framesize_callback(GLFWwindow *win, int w, int h);
void mouse_callback(GLFWwindow *win, double xpos, double ypos);
void processInput(GLFWwindow *win);
GLFWwindow *win = NULL;

Camera cam(vec3(0.0f));

float deltaTime, prevtime;

float vertices[] = {
	//     positions      // normals    // texture coords
	// Back
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	// Front
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	// Left
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	// Right
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	// Down
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	// Up
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

float c[] = {
	// front
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
	// back
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5};

unsigned int cI[] = {
	0, 1, 2, 2, 3, 0,
	// right
	1, 5, 6, 6, 2, 1,
	// back
	7, 6, 5, 5, 4, 7,
	// left
	4, 0, 3, 3, 7, 4,
	// bottom
	4, 5, 1, 1, 0, 4,
	// top
	3, 2, 6, 6, 7, 3};

int main(int argc, char **argv)
{
#pragma region GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	win = glfwCreateWindow(WIDTH, HEIGHT, "OPENGL_LEARN", NULL, NULL);

	if (win == NULL)
	{
		cout << "Cant Create GLFW Window\n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(win);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);

	// Callback for resize window
	glfwSetFramebufferSizeCallback(win, framesize_callback);
	glfwSetCursorPosCallback(win, mouse_callback);

	glfwSwapInterval(1);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable Aplha Channel
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion

#pragma region Init
	Shader shader("res\\basic.vert", "res\\basic.frag");
	Shader lightShader("res\\basic.vert", "res\\light.frag");
	Shader gridShader("res\\grid.vert", "res\\grid.frag");
	Shader modelShader("res\\model.vert", "res\\model.frag");
	VAO vao;
	vao.bind();

	VBO vbo(vertices, sizeof(vertices));

	// sending vertex data to VBOs
	vao.linkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	vao.linkVBO(vbo, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
	vao.linkVBO(vbo, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));

	// Binding VAO and VBO with 0 so accidently we cant change it
	vao.unbind();
	vbo.unbind();

	// Loading imgs to opengl Texture
	//  Texture tex("res\\brick.png");

	// Light
	VAO lightVao;

	lightVao.bind();
	lightVao.linkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	lightVao.unbind();

	// VAO gridVao;
	// gridVao.bind();
	// VBO quardVbo(quard, sizeof(quard));
	// gridVao.linkVBO(quardVbo, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	// gridVao.linkVBO(quardVbo, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));
	// gridVao.unbind();

	// tex.bind();

	cam.win = win;
	cam.lastX = WIDTH / 2.0f;
	cam.lastY = HEIGHT / 2.0f;
	cam.yawA = 3.0f;
	cam.pitchA = -40.0f;
	cam.front.z = 0.04f;
	// Matrices
	mat4 view = mat4(1.0f);

	mat4 projection = mat4(1.0f);
	projection = perspective(radians(70.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	// projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 1000.0f);
	//  projection = ortho(-(80.0f / 2.0f), 80.0f / 2.0f, 60.0f / 2.0f, -(60.0f / 2.0f), -1000.0f, 1000.0f);

	gridShader.activate();
	gridShader.setMat4("projection", value_ptr(projection));

	lightShader.activate();
	lightShader.setVec3("col", vec3(1.0f));
	lightShader.setMat4("projection", value_ptr(projection));

	modelShader.activate();
	modelShader.setMat4("projection", value_ptr(projection));

	shader.activate();
	shader.setVec3("objCol", vec3(1.0f, 0.2f, 0.1f));
	shader.setVec3("lightCol", vec3(1.0f));

	shader.setMat4("projection", value_ptr(projection));

#pragma endregion

	// cout << "ARG: " << argv[1] << '\n';
	// string s = argv[1];

#define SIZE 32
	bool chunk[SIZE][SIZE][SIZE] = {{{false}}};
	for (int z = 0; z < SIZE; z++)
		for (int y = 0 + z; y < SIZE - z; y++)
			for (int x = 0 + z; x < SIZE - z; x++)
			{
				chunk[x][z][y] = true;
			}

	vector<Vertex> ve;
	vector<uint32_t> id;

	cout << chunk[2][2][2] << '\n';
	// for (int x = 0; x < SIZE; x++)
	// 	for (int y = 0; y < SIZE; y++)
	// 		for (int z = 0; z < SIZE; z++)
	// 		{
	// 			if (!chunk[x][y][z])
	// 				continue;
	// 			if (x == 0)
	// 				updateVertex(ve, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
	// 			if (y == 0)
	// 				updateVertex(ve, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
	// 			if (z == 0)
	// 				updateVertex(ve, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
	// 			if (x == SIZE - 1)
	// 				updateVertex(ve, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
	// 			if (y == SIZE - 1)
	// 				updateVertex(ve, getFaceMesh(FaceSide::UP), vec3(x, y, z));
	// 			if (z == SIZE - 1)
	// 				updateVertex(ve, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));

	// 			if (x > 0 && !chunk[x - 1][y][z])
	// 				updateVertex(ve, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
	// 			if (y > 0 && chunk[x][y - 1][z] == false)
	// 				updateVertex(ve, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
	// 			if (z > 0 && chunk[x][y][z - 1] == false)
	// 				updateVertex(ve, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
	// 			if (x + 1 < SIZE && chunk[x + 1][y][z] == false)
	// 				updateVertex(ve, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
	// 			if (y + 1 < SIZE && chunk[x][y + 1][z] == false)
	// 				updateVertex(ve, getFaceMesh(FaceSide::UP), vec3(x, y, z));
	// 			if (z + 1 < SIZE && chunk[x][y][z + 1] == false)
	// 				updateVertex(ve, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));
	// 		}

	Texture tex("res\\brick.png");
	vector<TextureInfo> t;
	TextureInfo ti;
	ti.id = tex.ID;
	ti.type = "texture_diffuse";
	t.push_back(ti);
	// TinyModel tst(s);
	Mesh ms(ve, id, t);

	Chunk ch;
	ch.setVoxels(chunk);
	ch.generateMesh();
	bool pressed = false;
	float c = 1;
	mat4 model = mat4(1.0f);

	bool isWire = false;
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		cam.update(deltaTime);

		GLenum enu = glGetError();
		if (enu != 0)
			cout << enu << '\n';

		glClearColor(0.49f, 0.67f, 0.95f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = cam.getViewMatrix();

		// Light Model
		lightVao.bind();

		mat4 lightModel = mat4(1.0f);
		lightModel = translate(lightModel, vec3(0.0f, -1.0f, 0.0f) * c);
		lightModel = scale(lightModel, vec3(1.0f * c));

		lightShader.activate();
		lightShader.setMat4("view", value_ptr(view));
		lightShader.setMat4("model", value_ptr(lightModel));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// shader.setVec3("objCol", vec3(0.8f, 0.8f, 0.8f));

		// // Floor
		// model = mat4(1.0f);
		// model = translate(model, vec3(0.0f));
		// model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		// model = scale(model, vec3(4.0f, 0.1f, 4.0f));
		// shader.setMat4("model", value_ptr(model));
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// vao.bind();

		shader.activate();
		// shader.setVec3("lightPos", vec3(3.0f, 1.0f, -1.0f) * c);
		shader.setVec3("lightPos", cam.pos);
		shader.setVec3("viewPos", cam.pos);
		shader.setMat4("view", value_ptr(view));

		// for (int x = 0; x < 4; x++)
		// 	for (int y = 0; y < 4; y++)
		// 		for (int z = 0; z < 4; z++)
		// 		{
		model = mat4(1.0f);
		model = translate(model, vec3(0.0f));
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		// model = scale(model, vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("objCol", vec3(1.0f, 0.2f, 0.1f));
		shader.setMat4("model", value_ptr(model));
		ch.draw(shader);
		// }

		vao.bind();
		model = mat4(1.0f);
		model = translate(model, vec3(3.0f, -1.0f, 0.0f));
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		model = scale(model, vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("objCol", vec3(0.2f, 0.2f, 0.8f));
		shader.setMat4("model", value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Backpack
		model = mat4(1.0f);
		model = translate(model, vec3(0.0f, 0.0f, 0.0f));
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		model = scale(model, vec3(1.0f));

		// modelShader.activate();
		shader.setMat4("model", value_ptr(model));
		// modelShader.setMat4("view", value_ptr(view));
		// ms.draw(modelShader);
		// tst.draw(modelShader);

		float curTime = glfwGetTime();
		deltaTime = curTime - prevtime;
		prevtime = curTime;

		if (!pressed && (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS))
		{
			isWire = !isWire;
			printf("Cam:- X:%.2f Y:%.2f z:%0.2f\n", cam.pos.x, cam.pos.y, cam.pos.z);
			printf("Dir:- X:%.2f Y:%.2f z:%0.2f\n", cam.yawA, cam.pitchA, cam.front.z);
			pressed = true;

			glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
			// c += 0.1f;
		}

		if (glfwGetKey(win, GLFW_KEY_C) == GLFW_RELEASE)
			pressed = false;
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(win, true);
		glfwSwapBuffers(win);
	}

	shader.deleteProgram();
	vbo.deleteVBO();
	vao.deleteVAO();

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}

void framesize_callback(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow *win, double xpos, double ypos)
{
	cam.processMouse(xpos, ypos);
}

// Front Back Z-AXIS
// UP DOWN    Y-AXIS
// RIGHT LEFT X-AXIS
