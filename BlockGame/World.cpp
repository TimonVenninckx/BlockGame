#include "World.h"
#include <iostream>

constexpr int renderDistance = 4;

constexpr int chunkSize = 16;
constexpr int chunkHeight = 50;


void World::test()
{

}

bool World::GenerateWorld(int seed)
{
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(.006f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(5);
    noise.SetFractalGain(.3f);
    noise.SetFractalWeightedStrength(.0f);


    noise1.SetSeed(seed);
    noise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise1.SetFrequency(.06f);
    noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise1.SetFractalOctaves(5);
    noise1.SetFractalGain(.3f);
    noise1.SetFractalWeightedStrength(.0f);


    noise2.SetSeed(seed);
    noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise2.SetFrequency(.006f);
    noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise2.SetFractalOctaves(5);
    noise2.SetFractalGain(.3f);
    noise2.SetFractalWeightedStrength(.0f);




    Chunk emptychunk;
    for (int x{ 0 - renderDistance}; x < renderDistance; x++) {
        for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
            chunks[{ x,z }] = emptychunk;
            chunks[{ x,z }].generateChunk(noise, noise2, chunkSize, chunkHeight, x * chunkSize, z * chunkSize);
        }
    }


    for (int x{ 0 - renderDistance }; x < renderDistance; x++) {
        for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
            chunks[{x, z}].generateVertices();
            chunks[{x,z}].generateVAO();
        }
    }


    return true; // testing something revert if not working
}

void World::render() {

    /*for (int x{ 0 - renderDistance }; x < renderDistance; x++) {
        for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
            chunks[{x, z}].render();
        }
    }*/

    for (auto i{ chunksTORenderVao.begin() }; i != chunksTORenderVao.end(); i++) {
        if (i->second) {
            i->second->generateVAO();
            i->second = nullptr;
        }
    }


    for (auto i{ chunks.begin() }; i != chunks.end(); i++) {
        i->second.render();
    }
    //printf("We have %i chunks\n", chunks.size());
}

std::vector<Vertex>& World::getAllVertices()
{
    return allVertices;
}

void World::loadNewChunks(glm::vec3 playerposition)
{
    int playerX = std::floor(playerposition.x / 16.f);
    int playerZ = std::floor(playerposition.z / 16.f);

    constexpr int playerRDistance = 8;
    for (int x{ playerX - playerRDistance }; x <= playerX + playerRDistance; x++) {
        for (int z{ playerZ - playerRDistance }; z <= playerZ + playerRDistance; z++) {
            if (chunks.find({ x, z}) == chunks.end()) {
                /*printf("Hello new chunk %i,%i\n", x, z);
                printf("Player at %i,%i\n",playerX,playerZ);*/
                chunks[{ x, z }] = Chunk();
                chunks[{ x, z }].generateChunk(noise, noise2, chunkSize, chunkHeight, x * chunkSize, z * chunkSize);
                chunks[{x, z}].generateVertices();
                
                chunksTORenderVao[{ x, z }] = &chunks[{ x, z }];
            }
            //chunks[{x, z}].render(); // TESTING
        }
    }
}
