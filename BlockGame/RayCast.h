#pragma once
#include <glm/glm.hpp>
#include "World.h"
#include <vector>


class RayCast{

public:
    RayCast(glm::vec3 startPos, glm::vec3 direction, float rayLength);

    glm::ivec3 CheckForBlock(World& world);

    static std::vector<Vertex> rays;

private:
    glm::vec3 m_StartPos;
    glm::vec3 m_Direction;
    float m_RayLength;
};

