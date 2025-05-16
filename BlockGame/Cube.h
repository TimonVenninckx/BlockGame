#pragma once
#include <glm/vec3.hpp>
#include "Vertex.h";
#include <vector>

class Cube
{
public:


	Cube(int _id = 0)
		: id{ _id } 
	{	}

	void setId(int _id) {
		id = _id;
	}

	const glm::ivec3& getPosition() {
		return position;
	}
	glm::vec3 getFPosition() {
		return position;
	}

	void setPosition(int x, int y, int z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}

	bool isAir() {
		return id == 0;
	}

	//
	// will later change all of these to emplace_back()
	//
	
	void addTopVertexData(std::vector<Vertex>& list) {
		Vertex corner1( 0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 1.f, 1.f);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y,-0.5f + position.z, 0.f, 1.f);
		Vertex corner3(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f);
		Vertex corner4( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f);


		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}
	void addBottomVertexData(std::vector<Vertex>& list) {
		Vertex corner1( 0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 1.f, 1.f);
		Vertex corner2(-0.5f + position.x,-0.5f + position.y,-0.5f + position.z, 0.f, 1.f);
		Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 0.f);
		Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 0.f);

		// flip around the order to make it counter clockwise;
		// remeber -z is forward;
		list.push_back(corner3);
		list.push_back(corner2);
		list.push_back(corner1);
		list.push_back(corner1);
		list.push_back(corner4);
		list.push_back(corner3);
	}
	void addLeftVertexData(std::vector<Vertex>& list) {
		Vertex corner1(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 0.f);
		Vertex corner3(-0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 0.f, 1.f);
		Vertex corner4(-0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 1.f, 1.f);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addRightVertexData(std::vector<Vertex>& list) {
		Vertex corner1(0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 0.f);
		Vertex corner2(0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f);
		Vertex corner3(0.5f + position.x, -0.5f + position.y, 0.5f + position.z, 0.f, 1.f);
		Vertex corner4(0.5f + position.x, -0.5f + position.y, -0.5f + position.z, 1.f, 1.f);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addForwardVertexData(std::vector<Vertex>& list) {
		Vertex corner1( 0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 1.f, 0.f);
		Vertex corner2(-0.5f + position.x, 0.5f + position.y, 0.5f + position.z, 0.f, 0.f);
		Vertex corner3(-0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 0.f, 1.f);
		Vertex corner4( 0.5f + position.x,-0.5f + position.y, 0.5f + position.z, 1.f, 1.f);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}

	void addBackwardVertexData(std::vector<Vertex>& list) {
		Vertex corner1(-0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 1.f, 0.f);
		Vertex corner2( 0.5f + position.x, 0.5f + position.y, -0.5f + position.z, 0.f, 0.f);
		Vertex corner3( 0.5f + position.x,-0.5f + position.y, -0.5f + position.z, 0.f, 1.f);
		Vertex corner4(-0.5f + position.x,-0.5f + position.y, -0.5f + position.z, 1.f, 1.f);

		list.push_back(corner1);
		list.push_back(corner2);
		list.push_back(corner3);
		list.push_back(corner3);
		list.push_back(corner4);
		list.push_back(corner1);
	}


private:
	glm::ivec3 position{};
	int id{ 0 };
	
};

