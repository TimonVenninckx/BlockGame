#pragma once
#include "Chunk.h"
#include <map>
#include <glm/vec2.hpp>
#include <FastNoiseLite.h>
#include <tuple>
#include <atomic>




typedef std::tuple<int, int> ChunkPosition;

class World
{
public:

    void test();

    bool GenerateWorld(int seed);

    std::vector<Vertex>& getAllVertices();

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

