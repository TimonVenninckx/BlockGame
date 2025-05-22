#pragma once
#include <glm/vec3.hpp>
#include <glad/glad.h>

struct Vertex {
	glm::vec3 position;
	glm::vec3 TexCoord;
	Vertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat u, GLfloat v, GLfloat w) {
	    position.x = x_;
	    position.y = y_;
	    position.z = z_;
	    TexCoord.x = u;
	    TexCoord.y = v;
	    TexCoord.z = w;
	}
};

