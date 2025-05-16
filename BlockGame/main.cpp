#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Cube.h"


#include "PerlinNoise.h"

glm::vec3 cameraPos = glm::vec3(-10.f, 15.f, 15.f);
glm::vec3 cameraFront = glm::normalize(glm::vec3(cos(glm::radians(0.0f)) * cos(glm::radians(0.0f)), sin(glm::radians(0.0f)), sin(glm::radians(0.0f)) * cos(glm::radians(0.0f))));
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view = glm::mat4(1.f);


bool mouseLocked = false;


float yaw = 0.0f;
float pitch = 0.0f;
float fov = 45.f;


float deltatime = 0.0f;
float movespeed = 20.f;

float lastX = 400, lastY = 300;


void generateNoiseMap(std::vector<int>& chunk,int chunksize,int chunkheight, int startX, int startZ) {

	chunk.clear();
	chunk.reserve(chunksize * chunksize);


	int GRID_SIZE = 40;
	float highest = 0.f;
	float lowest = 0.f;

	for (int x{ 0 }; x < chunksize; x++) {
		for (int z{ 0 }; z < chunksize; z++) {

			float val = 0.f;

			float freq = 1.f;
			float amp = 1.f;


			for (int i{ 0 }; i < 4; i++) {
				val += PerlinNoise::perlin(x * freq / GRID_SIZE, z * freq / GRID_SIZE) * amp;

				freq *= 2.f;
				amp /= 2.f;
			}

			val *= 1.2f;

			val += 1.f;
			val *= .5f;
			// turn value from -1 to 1 into 0-1

			val *= chunkheight;
			chunk.push_back(val);

			//std::cout << val << '\n';
			if (val > highest)
				highest = val;
			if (val < lowest)
				lowest = val;
		}
	}
	std::cout << " HIGHEST VALUE = " << highest << '\n';
	std::cout << " HIGHEST VALUE = " << lowest << '\n';
}





void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		mouseLocked = !mouseLocked;
		if (mouseLocked) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	static bool firstmouse = true;

	if (firstmouse) {
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch -= yoffset; // invert

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += movespeed * deltatime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= movespeed * deltatime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos -= movespeed * deltatime * glm::cross(cameraUp, cameraFront);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += movespeed * deltatime * glm::cross(cameraUp, cameraFront);
	// up movement
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= movespeed * deltatime * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += movespeed * deltatime * cameraUp;

}

std::vector<Vertex> cubeVertices = {
	{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
	{ 0.5f, -0.5f, -0.5f,  1.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
	{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
	
	{-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
	{ 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{-0.5f,  0.5f,  0.5f,  0.0f, 1.0f},
	{-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},

	{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{-0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
	{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f},

	{ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{ 0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
	{ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f},

	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
	{ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
	{ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
	{-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},

	{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
	{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
	{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
	{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
	/*
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	*/
};



Vertex VERTICES[] = {		// textcoords
	Vertex( 0.5, 0.5f, 1.0f		,1.0f, 1.0f),
	Vertex( 0.5,-0.5f, 1.0f		,1.0f, 0.0f),
	Vertex(-0.5,-0.5f, 1.0f		,0.0f, 0.0f),
	Vertex(-0.5, 0.5f, 1.0f		,0.0f, 1.0f)
};


unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};


void ShowStatWindow(GLFWwindow* window) {
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	static float f0 = 0.001f;
	//ImGui::InputFloat("input float", &GLOBALHEIGHT, 0.1f, 1.0f, "%.3f");

	ImGui::DragFloat("xCoord", &cameraPos.x, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("yCoord", &cameraPos.y, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("zCoord", &cameraPos.z, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("yaw", &yaw, 1.f, -360.f, 360.f, "%.3f");
	ImGui::DragFloat("pitch", &pitch, 1.f, -89.f, 89.f, "%.3f");
	ImGui::DragFloat("fov", &fov, 1.f, 1.0f, 45.f, "%.3f");

	static bool culling = true;
	if (ImGui::Checkbox("culling", &culling)) {
		if (culling) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}

	if (ImGui::Checkbox("mouselocked", &mouseLocked)) {
		if (mouseLocked) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}


ImGuiIO& startImgui(GLFWwindow* window) {

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	return io;
}


int main() {
	if (!glfwInit())
	{
		printf("GLFW Initialization failed\n");
		exit(0);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1080, 720, "CubeGame", NULL,
		NULL);
	//glfwSetWindowPos(window, 2000, 200);

	if (!window)
	{
		printf("GLFW Window failed\n");
		exit(0);
	}
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Initializing GLAD failed\n");
		exit(0);
	}


	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
	

	ImGuiIO& io = startImgui(window);


	Shader Shader("shaders/shader.vs", "shaders/shader.fs");
	Shader.use();

	GLuint modelLoc = glGetUniformLocation(Shader.ID, "model");
	GLuint viewLoc = glGetUniformLocation(Shader.ID, "view");
	GLuint projectionLoc = glGetUniformLocation(Shader.ID, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	

	view = glm::rotate(view, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
	view = glm::translate(view, glm::vec3(25.0f, -20.0f, -20.0f));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 500.0f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(proj));


	GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cubeVertices.size(), &cubeVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	
	
	// 
	// texture
	// 
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// minifying and magnifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("textures/dirt.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}
	stbi_image_free(data);

	data = stbi_load("textures/dirt.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}
	stbi_image_free(data);


	glEnable(GL_CULL_FACE);


	
	//
	// main loop
	//

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	srand(glfwGetTime());






	constexpr int chunksize =  256;
	constexpr int chunkheight = 20;

	std::vector<int> chunkheightvalues;
	generateNoiseMap(chunkheightvalues, chunksize,chunkheight, 0, 0);

	auto chunk = new Cube[chunksize][chunkheight][chunksize];
	

	// generate chunk 
	for (int x{ 0 }; x < chunksize; x++) {
		for (int y{ 0 }; y < chunkheight; y++) {
			for (int z{ 0 }; z < chunksize; z++) {
				if (y < chunkheightvalues[x * chunksize + z])
					chunk[x][y][z].setId(1);

				chunk[x][y][z].setPosition(x, y, z);
			}
		}
	}
	// generate visible vertices

	std::vector<Vertex> visibleVertices;

	for (int x{ 0 }; x < chunksize; x++) {
		for (int y{ 0 }; y < chunkheight; y++) {
			for (int z{ 0 }; z < chunksize; z++) {
				Cube& cube = chunk[x][y][z];
				if (cube.isAir())
					continue;
				// check all directions to see if we are next to air block
				// if we are we need to draw that sprite
				if (y + 1 == chunkheight) // always add blocks on the border of the chunk ?!?!?
					cube.addTopVertexData(visibleVertices);
				else if(chunk[x][y + 1][z].isAir()) {
					cube.addTopVertexData(visibleVertices);
				}
				if (y == 0) {
					cube.addBottomVertexData(visibleVertices);
				}
				else if (chunk[x][y - 1][z].isAir()) {
					cube.addBottomVertexData(visibleVertices);
				}

				if (x == 0) { // left check
					cube.addLeftVertexData(visibleVertices);
				}
				else if (chunk[x - 1][y][z].isAir()) {
					cube.addLeftVertexData(visibleVertices);
				}
				if (x + 1 == chunksize) { // right check
					cube.addRightVertexData(visibleVertices);
				}
				else if (chunk[x + 1][y][z].isAir()) {
					cube.addRightVertexData(visibleVertices);
				}
				
				if (z == 0) { // backwards check
					cube.addBackwardVertexData(visibleVertices);
				}
				else if (chunk[x][y][z - 1].isAir()) {
					cube.addBackwardVertexData(visibleVertices);
				}
				if (z + 1 == chunksize) { // forward check
					cube.addForwardVertexData(visibleVertices);
				}
				else if (chunk[x][y][z + 1].isAir()) {
					cube.addForwardVertexData(visibleVertices);
				}
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * visibleVertices.size(), &visibleVertices[0], GL_STATIC_DRAW);

	std::cout << "how many verts" << visibleVertices.size();



	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ShowStatWindow(window);

		static float lastFrame = 0.0f;
		float currentFrame = glfwGetTime();
		deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		Shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


		proj = glm::perspective(glm::radians(fov), (float)scrWidth / (float)scrHeight, 0.1f, 1000.0f);
		glUniformMatrix4fv(projectionLoc,1, GL_FALSE, glm::value_ptr(proj));


		glm::mat4 bruh = glm::mat4(1.f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bruh));

		glDrawArrays(GL_TRIANGLES, 0, visibleVertices.size());



		glBindVertexArray(0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			// Update and Render additional Platform Windows
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}
