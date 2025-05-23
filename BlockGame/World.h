#pragma once
#include "Chunk.h"
#include <map>
#include <glm/vec2.hpp>
#include <FastNoiseLite.h>
#include <tuple>
#include <atomic>





typedef std::pair<int, int> ChunkPosition;


inline ChunkPosition operator+(const ChunkPosition& lhs, const ChunkPosition& rhs) {
    return ChunkPosition{ lhs.first + rhs.first, lhs.second + rhs.second };
}


class World
{
public:

    void test();
    void updateChunkNeighbours(ChunkPosition pos);

    bool GenerateWorld(int seed);


    void loadNewChunks(glm::vec3 playerposition);

    void render();
    

private:
    std::map<ChunkPosition, Chunk> chunks;

    std::map<ChunkPosition,Chunk*> chunksTORenderVao;

    FastNoiseLite noise;
    FastNoiseLite noise1;
    FastNoiseLite noise2;
    std::vector<Vertex> allVertices;

};

