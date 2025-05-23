#pragma once
#include "Vertex.h"
#include "Cube.h"
#include <vector>
#include <FastNoiseLite.h>

class Chunk;


enum class NeighbourDirection {
    FRONT,
    LEFT,
    RIGHT,
    BACK
};

struct ChunkNeighbors {
    Chunk* front{};
    Chunk* left{};
    Chunk* right{};
    Chunk* back{};
};

class Chunk
{
public:
    

    void generateChunk(const FastNoiseLite& noise, const FastNoiseLite& noise2, int _chunksize, int _chunkheight, int xoffset,int zoffset);
    void bindVerticesToVao(); // has to be called on main thread;
    void generateVAO();

    void generateVertices();

    void render();

    void setNeighbours(const ChunkNeighbors& neighours);
    void notifyNeighbours();

    void updateNeighbour(NeighbourDirection direction, Chunk* newNeighbour);

    int getBlockId(const glm::ivec3& pos);

    bool needRebindVertices = true;
    bool vaoMade = false;
private:



    GLuint vbo,vao;
    int chunksize;
    int chunkheight;
    int xoffset;
    int zoffset;
    std::vector<Cube> cubes; // x,y,z
    int amountOfVertices{};
    std::vector<Vertex> visiblevertices;

    ChunkNeighbors neighbours;

};

