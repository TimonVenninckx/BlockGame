#pragma once
#include "Chunk.h"
#include <glm/vec2.hpp>
#include <FastNoiseLite.h>
#include <tuple>
#include <atomic>
#include <map>




typedef std::pair<int, int> ChunkPosition;


inline ChunkPosition operator+(const ChunkPosition& lhs, const ChunkPosition& rhs) {
    return ChunkPosition{ lhs.first + rhs.first, lhs.second + rhs.second };
}


class World
{
public:
    static const int ChunkSize;
    static const int ChunkHeight;


    void Test();
    void UpdateChunkNeighbours(ChunkPosition pos);

    bool GenerateWorld(int seed);


    void LoadNewChunks(glm::vec3 playerPosition);

    void Render();
    
    
    std::map<ChunkPosition, Chunk>& getChunks();

private:
    //std::map<ChunkPosition, Chunk> chunks;
    std::map<ChunkPosition, Chunk> m_Chunks;
    std::map<ChunkPosition,Chunk*> m_ChunksToRenderVao;

    FastNoiseLite m_Noise;
    FastNoiseLite m_Noise1;
    FastNoiseLite m_Noise2;
    std::vector<Vertex> m_AllVertices;

};

