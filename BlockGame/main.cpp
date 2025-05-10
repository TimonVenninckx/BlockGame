#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	 // test

	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
						  		
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f
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

struct Vertex {
	glm::fvec3 position;
	GLfloat TexCoord[2];
	Vertex(GLfloat x_, GLfloat y_,GLfloat z_, GLfloat t1, GLfloat t2) {
		position.x = x_;
		position.y = y_;
		position.z = z_;
		TexCoord[0] = t1;
		TexCoord[1] = t2;
	}
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




int main() {
	if (!glfwInit())
	{
		printf("GLFW Initialization failed\n");
		exit(0);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "CubeGame", NULL,
		NULL);

	if (!window)
	{
		printf("GLFW Window failed\n");
		exit(0);
	}
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Initializing GLAD failed\n");
		exit(0);
	}


	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
	
	Shader Shader("shaders/shader.vs", "shaders/shader.fs");
	Shader.use();

	GLuint modelLoc = glGetUniformLocation(Shader.ID, "model");
	GLuint viewLoc = glGetUniformLocation(Shader.ID, "view");
	GLuint projectionLoc = glGetUniformLocation(Shader.ID, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	glm::mat4 view = glm::mat4(1.0f);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
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
	unsigned char* data = stbi_load("textures/grass_block_snow.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED to load texture \n";
	}
	stbi_image_free(data);

	//
	// main loop
	//

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	srand(glfwGetTime());

	glm::mat4 base = glm::mat4(1.0f);
	std::vector<glm::mat4> chunk;

	for (int i{ 0 }; i < 50; i++) {
		for (int j{ 0 }; j < 50; j++) {
			chunk.emplace_back(glm::translate(base, glm::vec3((float)-i, (float)(rand() % 3), (float)-j)));
		}
	}
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (glm::mat4& pos : chunk) {
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pos));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glfwTerminate();
	return 0;
}