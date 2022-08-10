// CPP program to render a triangle using Shaders
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PerlinNoise.hpp>

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
void setChunk(float fac, vec3 worldPos, int i);

GLFWwindow *win = NULL;

Camera cam(vec3(15.16f, 48.04f, 36.89f));

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

bool cursorActive = false;

#define C_T 4
Chunk ch[C_T];

siv::PerlinNoise::seed_type seed = 123456u;
siv::PerlinNoise perlin{seed};

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
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Enable Aplha Channel
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows

	// io.ConfigViewportsNoTaskBarIcon = true;
	ImGui::StyleColorsDark();
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
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

#define SIZE 32

	setChunk(0.027, vec3(0.0f), 0);
	setChunk(0.027, vec3(SIZE, 0.0f, 0.0f), 1);
	setChunk(0.027, vec3(0, 0.0f, SIZE), 2);
	setChunk(0.027, vec3(SIZE, 0.0f, SIZE), 3);

	bool pressed = false;
	bool esPres = true;

	float c = 1;
	mat4 model = mat4(1.0f);

	bool isWire = false;
	float chunkFac = 0.1f;
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

		shader.activate();
		shader.setVec3("lightPos", cam.pos);
		shader.setVec3("viewPos", cam.pos);
		shader.setMat4("view", value_ptr(view));

		for (int i = 0; i < C_T; i++)
		{
			model = mat4(1.0f);
			vec3 p = vec3(0.0f);
			switch (i)
			{
			case 1:
				p = vec3((float)SIZE, 0.0f, 0.0f);
				break;
			case 2:
				p = vec3(0, 0.0f, (float)SIZE);
				break;
			case 3:
				p = vec3((float)SIZE, 0.0f, (float)SIZE);
				break;
			default:
				break;
			}

			model = translate(model, p);
			// model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
			// model = scale(model, vec3(1.0f, 1.0f, 1.0f));
			shader.setVec3("objCol", vec3(1.0f, 0.2f, 0.1f));
			shader.setMat4("model", value_ptr(model));
			ch[i].draw(shader);
		}
		vao.bind();
		model = mat4(1.0f);
		model = translate(model, vec3(3.0f, -1.0f, 0.0f));
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		model = scale(model, vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("objCol", vec3(0.2f, 0.2f, 0.8f));
		shader.setMat4("model", value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Props");
		float f;
		if (ImGui::SliderFloat("FACTOR", &f, 0.0f, 1.0f))
		{
			setChunk(f, vec3(0.0f), 0);
			setChunk(f, vec3(SIZE, 0.0f, 0.0f), 1);
			setChunk(f, vec3(0, 0.0f, SIZE), 2);
			setChunk(f, vec3(SIZE, 0.0f, SIZE), 3);

			// chunkFac = f;
			printf("%f \n", f);
		}

		if (ImGui::Button("CLICK HERE"))
		{
			seed = (uint32_t)(rand() * 156907892);
			cout << seed << '\n';
			perlin = siv::PerlinNoise{seed};

			setChunk(f, vec3(0.0f), 0);
			setChunk(f, vec3(SIZE, 0.0f, 0.0f), 1);
			setChunk(f, vec3(0, 0.0f, SIZE), 2);
			setChunk(f, vec3(SIZE, 0.0f, SIZE), 3);
		}
		ImGui::SameLine();
		ImGui::LabelText("Seed", ": %i", seed);

		ImGui::End();

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

		if (!esPres && (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS))
		{
			glfwSetInputMode(win, GLFW_CURSOR, cursorActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			cursorActive = !cursorActive;
			esPres = true;
		}

		if (glfwGetKey(win, GLFW_KEY_C) == GLFW_RELEASE)
			pressed = false;
		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_RELEASE)
			esPres = false;
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(win, true);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		glfwSwapBuffers(win);
	}

	shader.deleteProgram();
	vbo.deleteVBO();
	vao.deleteVAO();

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}

void setChunk(float fac, vec3 worldPos, int i)
{

	bool chunk[SIZE][SIZE][SIZE] = {{{false}}};

	printf(" X:%f Y:%f Z:%f \n", worldPos.x, worldPos.y, worldPos.z);
	for (int x = 0; x < SIZE; x++)
		for (int z = 0; z < SIZE; z++)
		{
			double n = perlin.octave2D_01((x + worldPos.x) * fac, (z + worldPos.z) * fac, 8);

			// printf("[ %i ] ", (int)(x + worldPos.x));
			int height = n * (SIZE - 1);
			// if (height >= SIZE || height < 0)
			// 	printf("H: %i n: %f\n", height, n);
			// if (n > 0.1)
			for (int y = 0; y < height; y++)
				chunk[x][y][z] = true;
		}
	ch[i].setVoxels(chunk);
	ch[i].generateMesh();
}

void framesize_callback(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow *win, double xpos, double ypos)
{
	if (!cursorActive)
		cam.processMouse(xpos, ypos);
}

// Front Back Z-AXIS
// UP DOWN    Y-AXIS
// RIGHT LEFT X-AXIS
