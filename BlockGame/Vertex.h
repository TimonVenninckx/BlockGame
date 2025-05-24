#pragma once
#include <glm/vec3.hpp>
#include <glad/glad.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 TexCoord;
    glm::vec3 Normal;
    Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v, GLfloat w, glm::vec3 normal)
        : Position{ x,y,z }
        , TexCoord{ u,v,w }
        , Normal{ normal }
    {}

    Vertex(glm::vec3 position, glm::vec3 texCoord, glm::vec3 normal)
        : Position{ position }
        , TexCoord{ texCoord }
        , Normal{ normal }
    {
    }
};

