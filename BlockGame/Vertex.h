#pragma once
#include <glm/vec3.hpp>
#include <glad/glad.h>

struct Vertex {
	glm::vec3 position;
	GLfloat TexCoord[2];
	Vertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat t1, GLfloat t2) {
		position.x = x_;
		position.y = y_;
		position.z = z_;
		TexCoord[0] = t1;
		TexCoord[1] = t2;
	}
};

