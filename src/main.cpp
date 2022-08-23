// CPP program to render a triangle using Shaders
#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <PerlinNoise.hpp>

#include "Input.h"
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
Chunk *setChunk(float fac, vec3 worldPos, int i);
void updateChunks(vec3 &pos, bool forceUpdate = false);
vec3 voxelRayCast(vec3 &chunpos, vec3 &pos, vec3 &front, int *side);

GLFWwindow *win = NULL;
Camera cam(vec3(15.16f, 48.04f, 36.89f));
Input *inp = NULL;

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

// 0.5f,  0.5f, 0.0f,  // top right
//  0.5f, -0.5f, 0.0f,  // bottom right
// -0.5f, -0.5f, 0.0f,  // bottom left
// -0.5f,  0.5f, 0.0f,};
float c[] = {
	0.5f, 0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,	// bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f};

bool cursorActive = false;

#define C_T 4
vector<Chunk> ch(C_T *C_T);
unordered_map<vec3, Chunk *> loadedC;

siv::PerlinNoise::seed_type seed = 123456u;
siv::PerlinNoise perlin{seed};
float factor = 0.027f;

vec2 loadPos = vec2(0, 0);

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
	// glfwSetCursorPosCallback(win, mouse_callback);

	glfwSwapInterval(1);
	// glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// glfwSetInputMode(win, GLFW_CURSOR, cursorActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

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
	Shader shader("./res/basic.vert", "./res/basic.frag");
	Shader lightShader("./res/basic.vert", "./res/light.frag");
	Shader orthoShader("./res/2d.vs", "./res/2d.fs");
	Shader modelShader("./res/model.vert", "./res/model.frag");

	VAO vao;
	vao.bind();
	VBO vbo(vertices, sizeof(vertices));
	vao.linkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	vao.linkVBO(vbo, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
	vao.linkVBO(vbo, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
	vao.unbind();
	vbo.unbind();

	// Loading imgs to opengl Texture
	//  Texture tex("res\\brick.png");

	// Light
	VAO lightVao;

	lightVao.bind();
	lightVao.linkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	lightVao.unbind();

	VAO quardVao;

	quardVao.bind();
	VBO quardVbo(c, sizeof(c));
	quardVao.linkVBO(quardVbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	quardVao.unbind();

	inp = Input::getInstance();
	inp->Init(win);

	cam.win = win;
	cam.lastX = WIDTH / 2.0f;
	cam.lastY = HEIGHT / 2.0f;
	cam.yawA = 3.0f;
	cam.pitchA = -40.0f;
	cam.front.z = 0.04f;
	// Matrices
	mat4 view = mat4(1.0f);

	mat4 projection = perspective(radians(60.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
	// projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 1000.0f);
	mat4 orthoproj = ortho(-(800.0f / 2.0f), 800.0f / 2.0f, 600.0f / 2.0f, -(600.0f / 2.0f), -1000.0f, 1000.0f);

	orthoShader.activate();
	orthoShader.setMat4("projection", value_ptr(orthoproj));

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

	updateChunks(cam.pos);

	bool pressed = false;
	bool esPres = true;

	float c = 1;
	mat4 model = mat4(1.0f);

	bool isWire = false;
	float chunkFac = 0.1f;

	int upPos = 0;
	bool upTog = true;

	float f = factor;

	bool camTog = true;
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(win))
	{
		inp->Update();
		glfwPollEvents();

		if (!cursorActive)
		{
			vec2 mp = inp->GetMousePos();
			cam.processMouse(mp.x, mp.y);
		}

		cam.update(deltaTime);

		GLenum enu = glGetError();
		if (enu != 0)
			cout << enu << '\n';

		// Check Pos
		// if (cursorActive)
		updateChunks(cam.pos);

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

		int dC = 0;

		vec3 p = vec3(floor(cam.pos.x / SIZE), 0, floor(cam.pos.z / SIZE));
		p = (float)SIZE * p;

		for (std::unordered_map<vec3, Chunk *>::iterator iter = loadedC.begin(); iter != loadedC.end(); ++iter)
		{
			model = mat4(1.0f);
			// vec3 p = vec3(loadPos.x + (x * SIZE), 0.0f, loadPos.y + (y * SIZE));
			model = translate(model, iter->first);
			model = scale(model, vec3(1.0f, 1.0f, 1.0f));
			shader.setVec3("objCol", vec3(1.0f, 0.2f, 0.1f));
			shader.setMat4("model", value_ptr(model));

			// Highlighting selected Chunk
			shader.setVec3("objCol", p == iter->first ? vec3(0.8f) : vec3(0.2f));
			iter->second->draw(shader);
			dC++;
		}

#pragma region IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Props");

		ImGui::LabelText("FPS:", "%f", 1 / deltaTime);

		// Noise settings
		if (ImGui::SliderFloat("FACTOR", &f, 0.0f, 1.0f))
		{
			factor = f;
			updateChunks(cam.pos, true);
			printf("%f \n", f);
		}

		if (ImGui::Button("CLICK HERE"))
		{
			seed = (uint32_t)(rand() * 156907892);
			cout << seed << '\n';
			perlin = siv::PerlinNoise{seed};

			updateChunks(cam.pos, true);
		}
		ImGui::SameLine();
		ImGui::LabelText("Seed", ": %li", seed);

		// Player Stats
		ImGui::LabelText("loadPos", "X: %.2f Z: %.2f ", loadPos.x, loadPos.y);
		ImGui::LabelText("PlayerPos", "X: %.2f Y: %.2f Z: %.2f ", cam.pos.x, cam.pos.y, cam.pos.z);

		ivec3 cp = cam.pos - p;

		int side = 0;
		vec3 ra = voxelRayCast(p, cam.pos, cam.front, &side);

		ivec3 selV = ra - p;
		char s = '0';
		switch (side)
		{
		case 0:
			selV.x += cam.front.x > 0 ? -1 : 1;
			s = 'X';
			break;
		case 1:
			selV.y += cam.front.y > 0 ? -1 : 1;
			s = 'Y';
			break;
		case 2:
			selV.z += cam.front.z > 0 ? -1 : 1;
			s = 'Z';
			break;
		default:
			break;
		}

		ImGui::LabelText("P", "X: %.2f Y: %.2f Z: %.2f ", p.x, p.y, p.z);

		ImGui::LabelText("Chunk Pos", "X: %i Y: %i Z: %i ", cp.x, cp.y, cp.z);
		ImGui::LabelText("Sel Chunk Pos", "X: %i Y: %i Z: %i ", selV.x, selV.y, selV.z);
		ImGui::LabelText("SIDE", "%c", s);
		ImGui::LabelText("Draw Call", "%i", dC);

		ImGui::End();
#pragma endregion

		float curTime = glfwGetTime();
		deltaTime = curTime - prevtime;
		prevtime = curTime;

#pragma region Inputs
		if (!pressed && (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS))
		{
			isWire = !isWire;
			printf("Cam:- X:%.2f Y:%.2f z:%0.2f\n", cam.pos.x, cam.pos.y, cam.pos.z);
			printf("Dir:- X:%.2f Y:%.2f z:%0.2f\n", cam.yawA, cam.pitchA, cam.front.z);
			pressed = true;

			Chunk *selChunk = loadedC.at(p);

			Chunk *newC = new Chunk();
			newC->setVoxels(selChunk->voxels);
			newC->updateBlock(0, 0, 0, isWire);
			newC->generateMesh();

			delete selChunk;
			loadedC.erase(p);
			loadedC.emplace(p, newC);

			// c += 0.1f;
		}

		if (!esPres && (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS))
		{
			glfwSetInputMode(win, GLFW_CURSOR, cursorActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			cursorActive = !cursorActive;
			esPres = true;
		}

		if (glfwGetKey(win, GLFW_KEY_C) == GLFW_RELEASE)
			pressed = false;
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
			esPres = false;
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(win, true);
#pragma endregion

		vao.bind();
		model = mat4(1.0f);
		// vec3 nc = p + (vec3)cp ;
		model = translate(model, ra);
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		model = scale(model, vec3(1.2f));
		shader.setVec3("objCol", vec3(0.2f, 0.2f, 0.8f));
		shader.setMat4("model", value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = mat4(1.0f);
		// vec3 nc = p + (vec3)cp ;
		model = translate(model, (vec3)selV + p);
		model = rotate(model, radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
		model = scale(model, vec3(1.2f));
		shader.setVec3("objCol", vec3(0.2f, 0.2f, 0.8f));
		shader.setMat4("model", value_ptr(model));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// toggle camera for player to creative
		if (inp->isKeyDown(GLFW_KEY_P))
		{
			cout << "p\n";
			cam.cameraSpeed = camTog ? 1.0f : 12.0f;
			// cam.cameraSpeed = 12.0f;
			camTog = !camTog;
		}

		if (inp->isKeyDown(GLFW_KEY_M))
		{
			Chunk *selChunk = loadedC.at(p);

			Chunk *newC = new Chunk();
			newC->setVoxels(selChunk->voxels);

			cout << "CP: " << selChunk->voxels[selV.x][selV.y][selV.z] << '\n';
			newC->updateBlock(selV.x, selV.y, selV.z, true);
			newC->generateMesh();

			delete selChunk;
			loadedC.erase(p);
			loadedC.emplace(p, newC);
		}

		quardVao.bind();
		orthoShader.activate();
		model = mat4(1.0f);
		model = translate(model, vec3(1.0f));
		orthoShader.setMat4("model", value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

Chunk *setChunk(float fac, vec3 worldPos, int i)
{
	Chunk *chunkM = new Chunk();
	bool chunk[SIZE][SIZE][SIZE] = {{{false}}};

	if (worldPos != vec3(0.0f))
		for (int x = 0; x < SIZE; x++)
			for (int z = 0; z < SIZE; z++)
			{
				double n = perlin.octave2D_01((x + worldPos.x) * fac, (z + worldPos.z) * fac, 8);

				int height = n * (SIZE - 1);
				for (int y = 0; y < height; y++)
					chunk[x][y][z] = true;
			}
	chunkM->setVoxels(chunk);
	chunkM->generateMesh();

	return chunkM;
}

void updateChunks(vec3 &pos, bool forceUpdate)
{
	vec2 p = vec3(floor(pos.x / SIZE), 0, floor(pos.z / SIZE));
	p = (float)SIZE * p;

	int count = 0;
	if (forceUpdate)
	{
		loadPos = p;

		// delete all chunks
		for (auto it = loadedC.cbegin(), next_it = it; it != loadedC.cend(); it = next_it)
		{
			++next_it;
			delete loadedC.at(it->first);
			loadedC.erase(it);
		}

		for (int y = -C_T; y < C_T; y++)
			for (int x = -C_T; x < C_T; x++)
			{
				// vec3 p = vec3(loadPos.x + (x * SIZE), 0.0f, loadPos.y + (y * SIZE));
				vec3 p = vec3(loadPos.x + (x * SIZE), 0.0f, loadPos.y + (y * SIZE));
				Chunk *c = setChunk(factor, p, x + (y * C_T));
				loadedC.emplace(p, c);
				count++;
			}

		cout << "Force TotallLoad: " << count << '\n';
	}
	if (loadPos != p)
	{
		loadPos = p;

		// delete all chunks that are away from player
		for (auto it = loadedC.cbegin(), next_it = it; it != loadedC.cend(); it = next_it)
		{
			++next_it;
			if (abs(distance(pos, it->first)) > C_T * SIZE)
			{
				// printf("Remove Chunk: %f : %.2f %.2f %.2f \n", distance(pos, it->first), it->first.x, it->first.y, it->first.z);
				delete loadedC.at(it->first);
				loadedC.erase(it);
			}
		}

		for (int y = -C_T; y < C_T; y++)
			for (int x = -C_T; x < C_T; x++)
			{
				// vec3 p = vec3(loadPos.x + (x * SIZE), 0.0f, loadPos.y + (y * SIZE));
				vec3 p = vec3(loadPos.x + (x * SIZE), 0.0f, loadPos.y + (y * SIZE));
				if (!loadedC.count(p))
				{
					Chunk *c = setChunk(factor, p, x + (y * C_T));
					loadedC.emplace(p, c);
					count++;
				}
			}

		cout << "TotallLoad: " << count << '\n';
	}
}

vec3 voxelRayCast(vec3 &chunpos, vec3 &pos, vec3 &front, int *side)
{
	Chunk *chun = loadedC.at(chunpos);

	vec3 ray = pos - chunpos; // Get ray origin releative to chunk
	vec3 dir = front;

	vec3 deltaDis;
	deltaDis.x = dir.x == 0 ? 1e30 : abs(1 / dir.x);
	deltaDis.y = dir.y == 0 ? 1e30 : abs(1 / dir.y);
	deltaDis.z = dir.z == 0 ? 1e30 : abs(1 / dir.z);

	ivec3 Step(0);
	vec3 sideDis; // tMax
	// Init Step
	if (dir.x > 0)
	{
		Step.x = 1;
		sideDis.x = (floor(ray.x) + 1.0 - ray.x) * deltaDis.x;
	}
	else if (dir.x < 0)
	{
		Step.x = -1;
		sideDis.x = (ray.x - floor(ray.x)) * deltaDis.x;
	}

	if (dir.y > 0)
	{
		Step.y = 1;
		sideDis.y = (floor(ray.y) + 1.0 - ray.y) * deltaDis.y;
	}
	else if (dir.y < 0)
	{
		Step.y = -1;
		sideDis.y = (ray.y - floor(ray.y)) * deltaDis.y;
	}

	if (dir.z > 0)
	{
		Step.z = 1;
		sideDis.z = (floor(ray.z) + 1.0 - ray.z) * deltaDis.z;
	}
	else if (dir.z < 0)
	{
		Step.z = -1;
		sideDis.z = (ray.z - floor(ray.z)) * deltaDis.z;
	}

	bool hit = false;
	while (sideDis.x < SIZE || sideDis.y < SIZE || sideDis.z < SIZE)
	{
		if (sideDis.x < sideDis.y)
		{
			if (sideDis.x < sideDis.z)
			{
				// move to x
				ray.x += Step.x;
				sideDis.x += deltaDis.x;
				*side = 0;
			}
			else
			{
				// move to z
				ray.z += Step.z;
				sideDis.z += deltaDis.z;
				*side = 2;
			}
		}
		else
		{
			if (sideDis.y < sideDis.z)
			{
				// move to y
				ray.y += Step.y;
				sideDis.y += deltaDis.y;
				*side = 1;
			}
			else
			{
				// move to z
				ray.z += Step.z;
				sideDis.z += deltaDis.z;
				*side = 2;
			}
		}

		ivec3 mp = round(ray);
		if (chun->voxels[mp.x][mp.y][mp.z])
		{
			ImGui::LabelText("Ray", "x %.2f y %.2f z %.2f", ray.x, ray.y, ray.z);
			ImGui::LabelText("MP", "x %i y %i z %i", mp.x, mp.y, mp.z);
			return ((vec3)mp + chunpos);
		}
	}

	return ivec3(-1);
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
