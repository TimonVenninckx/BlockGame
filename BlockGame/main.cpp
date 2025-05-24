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

#include "World.h"
#include "RayCast.h"

#include <thread>

glm::vec3 g_cameraPos = glm::vec3(8.f, 160.f, 8.f);
glm::vec3 g_cameraFront = glm::normalize(glm::vec3(cos(glm::radians(0.0f)) * cos(glm::radians(0.0f)), sin(glm::radians(0.0f)), sin(glm::radians(0.0f)) * cos(glm::radians(0.0f))));
glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 g_view = glm::mat4(1.f);



bool g_mouseLocked = false;


float g_yaw = 0.0f;
float g_pitch = -89.f;
float g_fov = 45.f;


float g_deltatime = 0.0f;
float g_movespeed = 10.f;

int g_range = 6;

float g_lastX = 400, g_lastY = 300;

bool g_keeprendering = true;

World g_world;

void ParalelTest() {
    while (g_keeprendering) {
        g_world.LoadNewChunks(g_cameraPos);
    }
}


void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        g_mouseLocked = !g_mouseLocked;
        if (g_mouseLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    g_fov -= (float)xOffset;
    if (g_fov < 1.0f)
        g_fov = 1.0f;
    if (g_fov > 45.0f)
        g_fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

    static bool firstmouse = true;

    if (!g_mouseLocked) {
        firstmouse = true;
        return;
    }

    if (firstmouse) {
        g_lastX = (float)xPos;
        g_lastY = (float)yPos;
        firstmouse = false;
    }


    float xoffset = xPos - g_lastX;
    float yoffset = yPos - g_lastY;
    g_lastX = (float)xPos;
    g_lastY = (float)yPos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_yaw += xoffset;
    g_pitch -= yoffset; // invert

    if (g_pitch > 89.0f)
        g_pitch = 89.0f;
    if (g_pitch < -89.0f)
        g_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    g_cameraFront = glm::normalize(direction);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        RayCast ray(g_cameraPos, g_cameraFront, g_range);
        ray.CheckForBlock(g_world);
    }

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_cameraPos += g_movespeed * g_deltatime * g_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_cameraPos -= g_movespeed * g_deltatime * g_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_cameraPos -= g_movespeed * g_deltatime * glm::normalize(glm::cross(g_cameraUp, g_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_cameraPos += g_movespeed * g_deltatime * glm::normalize(glm::cross(g_cameraUp, g_cameraFront));
    // up movement
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        g_cameraPos -= g_movespeed * g_deltatime * g_cameraUp;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        g_cameraPos += g_movespeed * g_deltatime * g_cameraUp;

}


void ShowStatWindow(GLFWwindow* window) {
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	static float f0 = 0.001f;
	//ImGui::InputFloat("input float", &GLOBALHEIGHT, 0.1f, 1.0f, "%.3f");

	ImGui::DragFloat("xCoord", &g_cameraPos.x, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("yCoord", &g_cameraPos.y, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("zCoord", &g_cameraPos.z, .1f, -100.f, 100.f, "%.3f");
	ImGui::DragFloat("yaw", &g_yaw, 1.f, -360.f, 360.f, "%.3f");
	ImGui::DragFloat("pitch", &g_pitch, 1.f, -89.f, 89.f, "%.3f");
	ImGui::DragFloat("fov", &g_fov, 1.f, 1.0f, 45.f, "%.3f");
	ImGui::DragFloat("movespeed", &g_movespeed, 5.f, 5.f, 500.f, "%.3f");

    ImGui::DragInt("Range", &g_range, 1.f, 3, 20);

    if (ImGui::SmallButton("Raycast")) {
        RayCast ray(g_cameraPos, g_cameraFront, g_range);
        ray.CheckForBlock(g_world);
    }

	static bool culling = true;
	if (ImGui::Checkbox("culling", &culling)) {
		if (culling) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}

	if (ImGui::Checkbox("mouselocked", &g_mouseLocked)) {
		if (g_mouseLocked) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}


ImGuiIO& StartImgui(GLFWwindow* window) {

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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "CubeGame", NULL,
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Initializing GLAD failed\n");
		exit(0);
	}


	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
	

	ImGuiIO& io = StartImgui(window);


	Shader Shader("shaders/shader.vs", "shaders/shader.fs");
	Shader.use();

	GLuint modelLoc = glGetUniformLocation(Shader.ID, "model");
	GLuint viewLoc = glGetUniformLocation(Shader.ID, "view");
	GLuint projectionLoc = glGetUniformLocation(Shader.ID, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	

	g_view = glm::rotate(g_view, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
	g_view = glm::translate(g_view, glm::vec3(25.0f, -20.0f, -20.0f));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 5000.0f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(g_view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(proj));


	
	// 
	// texture
	// 

	
	stbi_set_flip_vertically_on_load(true);

	GLuint texturetest;

	GLsizei width = 16;
	GLsizei height = 16;
	GLsizei layercount = 3;
	GLsizei nrChannels;



	glGenTextures(1, &texturetest);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texturetest);
	
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width, height, layercount, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	

	// minifying and magnifying
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /// 
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	unsigned char* data = stbi_load("texturepack/assets/minecraft/textures/block/dirt.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}

	stbi_image_free(data);

	data = stbi_load("texturepack/assets/minecraft/textures/block/grass_block_side.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}

	stbi_image_free(data);

	data = stbi_load("textures/grass_block_top_changed.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 2, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}

	stbi_image_free(data);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	

    //
    // ray cast debugging uses
    // 

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	//
	// main loop
	//







	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	srand((unsigned int)glfwGetTime());


    int seed = 99;
    g_world.GenerateWorld(seed);
    



    // turn world gen off for a sec
    std::thread worker(ParalelTest);

    //g_world.LoadNewChunks(g_cameraPos);

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		ProcessInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ShowStatWindow(window);

		static float lastFrame = 0.0f;
		float currentFrame = glfwGetTime();
        g_deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		Shader.use();
		glBindTexture(GL_TEXTURE_2D_ARRAY, texturetest);


		glm::mat4 g_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);


		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(g_view));

		proj = glm::perspective(glm::radians(g_fov), (float)scrWidth / (float)scrHeight, .01f, 5000.0f);
		glUniformMatrix4fv(projectionLoc,1, GL_FALSE, glm::value_ptr(proj));


        g_world.Render();

        if (RayCast::rays.size() > 0) {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * RayCast::rays.size(), &RayCast::rays[0], GL_STATIC_DRAW);
            glDrawArrays(GL_LINES, 0, RayCast::rays.size());
            glBindVertexArray(0);
        }

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
    g_keeprendering = false;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

    worker.join();
	return 0;
}


/*


std::vector<Vertex> cubeVertices = {
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f},

    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f},
    { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.f},
    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f},

    {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f},
    {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},

    { 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    { 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f},
    { 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},

    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    { 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},
    { 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f},
    { 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f},

    {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f},
    {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f},
    {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f},

};


    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cubeVertices.size(), &cubeVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //// indices
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);





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
	*/