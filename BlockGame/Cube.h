#pragma once
#include <glm/vec3.hpp>
#include "Vertex.h"
#include <vector>

class Cube
{
public:


	Cube(uint8_t _id = 0)
		: id{ _id } 
	{	}

	void setId(uint8_t _id) {
		id = _id;
		if (id == 1) {
			topTextureId = 0;
			sideTextureId = 0;
		}
		else if (id == 2) {
			topTextureId = 2;
			sideTextureId = 1;
		}
	}

	/*const glm::ivec3& getPosition() {
		return position;
	}
	glm::vec3 getFPosition() {
		return position;
	}

	void setPosition(int x, int y, int z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}*/

	bool isAir() {
		return id == 0;
	}

	//
	// will later change all of these to emplace_back()
	//
	
	void addTopVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1( 0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f, topTextureId);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f, topTextureId);
		Vertex corner3(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId);
		Vertex corner4( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId);


		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}
	void addBottomVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1( 0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 1.f, topTextureId);
		Vertex corner2(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 1.f, topTextureId);
		Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, topTextureId);
		Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, topTextureId);

		// flip around the order to make it counter clockwise;
		// remeber -z is forward;
		list.push_back(corner3);
		list.push_back(corner2);
		list.push_back(corner1);
		list.push_back(corner1);
		list.push_back(corner4);
		list.push_back(corner3);
	}
	void addLeftVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f, sideTextureId);
		Vertex corner3(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 0.f, sideTextureId);
		Vertex corner4(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addRightVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1(0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f, sideTextureId);
		Vertex corner2(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId);
		Vertex corner3(0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId);
		Vertex corner4(0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 0.f, sideTextureId);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addForwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 1.f, sideTextureId);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 1.f, sideTextureId);
		Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f, sideTextureId);
		Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f, sideTextureId);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addBackwardVertexData(std::vector<Vertex>& list, glm::vec3 position) {
		Vertex corner1(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 1.f, sideTextureId);
		Vertex corner2( 0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 1.f, sideTextureId);
		Vertex corner3( 0.5f + position.x,-0.5f + position.y, -0.5f + position.z, 0.f, 0.f, sideTextureId);
		Vertex corner4(-0.5f + position.x,-0.5f + position.y, -0.5f + position.z, 1.f, 0.f, sideTextureId);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}


private:
	//glm::ivec3 position{};
    uint8_t id{ 0 };
    uint8_t topTextureId{ 1 };
    uint8_t sideTextureId{ 0 };
    
};

