#include "Cube.h"


/*

old version

void Cube::addTopVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 1.f, topTextureId);
    Vertex corner2(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 1.f, topTextureId);
    Vertex corner3(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId);
    Vertex corner4(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId);


    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}
void Cube::addBottomVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 1.f, 1.f, topTextureId);
    Vertex corner2(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 0.f, 1.f, topTextureId);
    Vertex corner3(-0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId);
    Vertex corner4(0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId);

    // flip around the order to make it counter clockwise;
    // remeber -z is forward;
    list.push_back(corner3);
    list.push_back(corner2);
    list.push_back(corner1);
    list.push_back(corner1);
    list.push_back(corner4);
    list.push_back(corner3);
}
void Cube::addLeftVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId);
    Vertex corner2(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 1.f, sideTextureId);
    Vertex corner3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 0.f, 0.f, sideTextureId);
    Vertex corner4(-0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId);

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

void Cube::addRightVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 1.f, sideTextureId);
    Vertex corner2(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId);
    Vertex corner3(0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId);
    Vertex corner4(0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 1.f, 0.f, sideTextureId);

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

void Cube::addForwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId);
    Vertex corner2(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId);
    Vertex corner3(-0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId);
    Vertex corner4(0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId);

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

void Cube::addBackwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    Vertex corner1(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 1.f, sideTextureId);
    Vertex corner2(0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 1.f, sideTextureId);
    Vertex corner3(0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 0.f, 0.f, sideTextureId);
    Vertex corner4(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 1.f, 0.f, sideTextureId);

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}


*/