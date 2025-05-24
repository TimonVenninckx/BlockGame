#pragma once
#include <glm/vec3.hpp>
#include "Vertex.h"
#include <vector>

class Cube
{
public:


	Cube(uint8_t id = 0)
		: Id { id }
	{	}

	void setId(uint8_t id) {
        Id = id;
		if (Id == 1) {
			TopTextureId = 0;
			SideTextureId = 0;
		}
		else if (Id == 2) {
			TopTextureId = 2;
			SideTextureId = 1;
		}
        else if (Id == 3) {
            TopTextureId = 3;
            SideTextureId = 3;
        }
	}

    uint8_t getId() {
        return Id;
    }
	bool isAir() {
		return Id == 0;
	}

	//
	// will later change all of these to emplace_back()
	//
	
    void addTopVertexData(std::vector<Vertex>& list, glm::vec3 position);
    void addBottomVertexData(std::vector<Vertex>& list, glm::vec3 position);
    void addLeftVertexData(std::vector<Vertex>& list, glm::vec3 position);
    void addRightVertexData(std::vector<Vertex>& list, glm::vec3 position);
    void addForwardVertexData(std::vector<Vertex>& list, glm::vec3 position);
    void addBackwardVertexData(std::vector<Vertex>& list, glm::vec3 position);


private:
	//glm::ivec3 position{};
    uint8_t Id{ 0 };
    uint8_t TopTextureId{ 1 };
    uint8_t SideTextureId{ 0 };
    
};


inline void Cube::addTopVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(1.0f + position.x, 1.0f + position.y, 0.0f + position.z, 1.f, 1.f, TopTextureId, glm::vec3(0.f, 1.f, 0.f));
    const Vertex corner2(0.0f + position.x, 1.0f + position.y, 0.0f + position.z, 0.f, 1.f, TopTextureId, glm::vec3(0.f, 1.f, 0.f));
    const Vertex corner3(0.0f + position.x, 1.0f + position.y, 1.0f + position.z, 0.f, 0.f, TopTextureId, glm::vec3(0.f, 1.f, 0.f));
    const Vertex corner4(1.0f + position.x, 1.0f + position.y, 1.0f + position.z, 1.f, 0.f, TopTextureId, glm::vec3(0.f, 1.f, 0.f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addBottomVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(1.0f + position.x, 0.0f + position.y, 0.0f + position.z, 1.f, 1.f, TopTextureId, glm::vec3(0.f, -1.f, 0.f));
    const Vertex corner2(0.0f + position.x, 0.0f + position.y, 0.0f + position.z, 0.f, 1.f, TopTextureId, glm::vec3(0.f, -1.f, 0.f));
    const Vertex corner3(0.0f + position.x, 0.0f + position.y, 1.0f + position.z, 0.f, 0.f, TopTextureId, glm::vec3(0.f, -1.f, 0.f));
    const Vertex corner4(1.0f + position.x, 0.0f + position.y, 1.0f + position.z, 1.f, 0.f, TopTextureId, glm::vec3(0.f, -1.f, 0.f));

    list.push_back(corner3);
    list.push_back(corner2);
    list.push_back(corner1);
    list.push_back(corner1);
    list.push_back(corner4);
    list.push_back(corner3);
}

inline void Cube::addLeftVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(0.0f + position.x, 1.0f + position.y, 1.0f + position.z, 1.f, 1.f, SideTextureId, glm::vec3(0.0f, 0.f, 0.f));
    const Vertex corner2(0.0f + position.x, 1.0f + position.y, 0.0f + position.z, 0.f, 1.f, SideTextureId, glm::vec3(0.0f, 0.f, 0.f));
    const Vertex corner3(0.0f + position.x, 0.0f + position.y, 0.0f + position.z, 0.f, 0.f, SideTextureId, glm::vec3(0.0f, 0.f, 0.f));
    const Vertex corner4(0.0f + position.x, 0.0f + position.y, 1.0f + position.z, 1.f, 0.f, SideTextureId, glm::vec3(0.0f, 0.f, 0.f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addRightVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(1.0f + position.x, 1.0f + position.y, 0.0f + position.z, 1.f, 1.f, SideTextureId, glm::vec3(1.0f, 0.f, 0.f));
    const Vertex corner2(1.0f + position.x, 1.0f + position.y, 1.0f + position.z, 0.f, 1.f, SideTextureId, glm::vec3(1.0f, 0.f, 0.f));
    const Vertex corner3(1.0f + position.x, 0.0f + position.y, 1.0f + position.z, 0.f, 0.f, SideTextureId, glm::vec3(1.0f, 0.f, 0.f));
    const Vertex corner4(1.0f + position.x, 0.0f + position.y, 0.0f + position.z, 1.f, 0.f, SideTextureId, glm::vec3(1.0f, 0.f, 0.f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addForwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(1.0f + position.x, 1.0f + position.y, 1.0f + position.z, 1.f, 1.f, SideTextureId, glm::vec3(0.f, 0.f, 1.0f));
    const Vertex corner2(0.0f + position.x, 1.0f + position.y, 1.0f + position.z, 0.f, 1.f, SideTextureId, glm::vec3(0.f, 0.f, 1.0f));
    const Vertex corner3(0.0f + position.x, 0.0f + position.y, 1.0f + position.z, 0.f, 0.f, SideTextureId, glm::vec3(0.f, 0.f, 1.0f));
    const Vertex corner4(1.0f + position.x, 0.0f + position.y, 1.0f + position.z, 1.f, 0.f, SideTextureId, glm::vec3(0.f, 0.f, 1.0f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addBackwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(0.0f + position.x, 1.0f + position.y, 0.0f + position.z, 1.f, 1.f, SideTextureId, glm::vec3(0.f, 0.f, 0.0f));
    const Vertex corner2(1.0f + position.x, 1.0f + position.y, 0.0f + position.z, 0.f, 1.f, SideTextureId, glm::vec3(0.f, 0.f, 0.0f));
    const Vertex corner3(1.0f + position.x, 0.0f + position.y, 0.0f + position.z, 0.f, 0.f, SideTextureId, glm::vec3(0.f, 0.f, 0.0f));
    const Vertex corner4(0.0f + position.x, 0.0f + position.y, 0.0f + position.z, 1.f, 0.f, SideTextureId, glm::vec3(0.f, 0.f, 0.0f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

/*

old version

inline void Cube::addTopVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1( 0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f, topTextureId, glm::vec3(0.f, 1.f,0.f));
    const Vertex corner2(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f, topTextureId, glm::vec3(0.f, 1.f,0.f));
    const Vertex corner3(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId, glm::vec3(0.f, 1.f,0.f));
    const Vertex corner4( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId, glm::vec3(0.f, 1.f,0.f));


    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}
inline void Cube::addBottomVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1( 0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 1.f, topTextureId, glm::vec3(0.f,-1.f,0.f));
    const Vertex corner2(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 1.f, topTextureId, glm::vec3(0.f,-1.f,0.f));
    const Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId, glm::vec3(0.f,-1.f,0.f));
    const Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId, glm::vec3(0.f,-1.f,0.f));

    // flip around the order to make it counter clockwise;
    // remeber -z is forward;
    list.push_back(corner3);
    list.push_back(corner2);
    list.push_back(corner1);
    list.push_back(corner1);
    list.push_back(corner4);
    list.push_back(corner3);
}
inline void Cube::addLeftVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId, glm::vec3(-0.5f, 0.f, 0.f));
    const Vertex corner2(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f, sideTextureId, glm::vec3(-0.5f, 0.f, 0.f));
    const Vertex corner3(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 0.f, sideTextureId, glm::vec3(-0.5f, 0.f, 0.f));
    const Vertex corner4(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId, glm::vec3(-0.5f, 0.f, 0.f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addRightVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f, sideTextureId, glm::vec3(0.5f, 0.f, 0.f));
    const Vertex corner2(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId, glm::vec3(0.5f, 0.f, 0.f));
    const Vertex corner3(0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId, glm::vec3(0.5f, 0.f, 0.f));
    const Vertex corner4(0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 0.f, sideTextureId, glm::vec3(0.5f, 0.f, 0.f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addForwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId, glm::vec3(0.f, 0.f, 0.5f));
    const Vertex corner2(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId, glm::vec3(0.f, 0.f, 0.5f));
    const Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId, glm::vec3(0.f, 0.f, 0.5f));
    const Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId, glm::vec3(0.f, 0.f, 0.5f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}

inline void Cube::addBackwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
    const Vertex corner1(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f, sideTextureId, glm::vec3(0.f, 0.f,-0.5f));
    const Vertex corner2( 0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f, sideTextureId, glm::vec3(0.f, 0.f,-0.5f));
    const Vertex corner3( 0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 0.f, sideTextureId, glm::vec3(0.f, 0.f,-0.5f));
    const Vertex corner4(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 0.f, sideTextureId, glm::vec3(0.f, 0.f,-0.5f));

    list.push_back(corner1);
    list.push_back(corner2);
    list.push_back(corner3);
    list.push_back(corner3);
    list.push_back(corner4);
    list.push_back(corner1);
}



*/