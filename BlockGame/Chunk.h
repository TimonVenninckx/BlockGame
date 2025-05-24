#pragma once
#include "Vertex.h"
#include "Cube.h"
#include <vector>
#include <FastNoiseLite.h>

class Chunk;


enum ENeighbourDirection {
    ENeighbourDirection_FRONT,
    ENeighbourDirection_LEFT,
    ENeighbourDirection_RIGHT,
    ENeighbourDirection_BACK
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
    

    void GenerateChunk(const FastNoiseLite& noise, const FastNoiseLite& noise2, int chunkSize, int chunkHeight, int xOffset,int zOffset);
    void BindVerticesToVao(); // has to be called on main thread;
    void GenerateVAO();

    void GenerateVertices();

    void Render();

    void SetNeighbours(const ChunkNeighbors& neighours);
    void NotifyNeighbours();

    void UpdateNeighbour(ENeighbourDirection direction, Chunk* newNeighbour);


    int GetBlockId(const glm::ivec3& pos);
    void RemoveBlock(const glm::ivec3& pos);

    bool NeedRebindVertices() const;
    bool VaoMade() const;

private:


    bool m_needRebindVertices = true;
    bool m_vaoMade = false;

    GLuint m_vbo, m_vao;
    int m_chunksize;
    int m_chunkheight;
    int m_xoffset;
    int m_zoffset;
    std::vector<Cube> m_cubes; // x,y,z
    int m_amountOfVertices{};
    std::vector<Vertex> m_visiblevertices;

    ChunkNeighbors m_neighbours;

};

