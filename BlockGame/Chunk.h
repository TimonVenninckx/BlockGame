#pragma once
#include "Vertex.h"
#include "Cube.h"
#include <vector>
#include <FastNoiseLite.h>


struct ChunkNeighbors {
    Chunk* front;
    Chunk* left;
    Chunk* right;
    Chunk* back;
};

class Chunk
{
public:
    
    void generateChunk(const FastNoiseLite& noise, const FastNoiseLite& noise2, int _chunksize, int _chunkheight, int xoffset,int zoffset);
    void generateVAO();
    void generateVertices();

    void render();

    void setNeighbours(const ChunkNeighbors& neighours);

private:
    GLuint vbo,vao;
    int chunksize;
    int chunkheight;
    int xoffset;
    int zoffset;
    std::vector<Cube> cubes; // x,y,z
    int amountOfVertices;
    std::vector<Vertex> visiblevertices;


    ChunkNeighbors neighbours;
};

