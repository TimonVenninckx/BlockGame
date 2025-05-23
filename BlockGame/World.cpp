#include "World.h"
#include <iostream>
#include <GLFW/glfw3.h>

constexpr int renderDistance = 0;

constexpr int chunkSize = 16;
constexpr int chunkHeight = 50;


void World::test()
{
    for (auto i{ chunks.begin() }; i != chunks.end(); i++) {
        updateChunkNeighbours(i->first);
    }
}

void World::updateChunkNeighbours(ChunkPosition chunkLocation)
{
    if (chunks.find(chunkLocation) == chunks.end()) {
        printf("Could not find chunk :: updateChunkNeighbours\n");
        return;
    }

    //printf("Position = %i:%i\n", chunkLocation.first, chunkLocation.second);

    ChunkNeighbors neighBours;

    ChunkPosition pos = chunkLocation;
    pos = chunkLocation + ChunkPosition{ 0,-1 }; // backwards
    if (chunks.find(pos) == chunks.end()) {
        neighBours.back = nullptr;
    }
    else {
        neighBours.back = &chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ 0,1 }; // forwards
    if (chunks.find(pos) == chunks.end()) {
        neighBours.front = nullptr;
    }
    else {
        neighBours.front = &chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ 1,0 }; // right
    if (chunks.find(pos) == chunks.end()) {
        neighBours.right = nullptr;
    }
    else {
        neighBours.right = &chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ -1,0 }; // left
    if (chunks.find(pos) == chunks.end()) {
        neighBours.left = nullptr;
    }
    else {
        neighBours.left = &chunks[pos];
    }
    chunks[chunkLocation].setNeighbours(neighBours);
}

bool World::GenerateWorld(int seed)
{
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(.006f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(1);
    noise.SetFractalGain(.3f);
    noise.SetFractalWeightedStrength(.0f);


    noise1.SetSeed(seed);
    noise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise1.SetFrequency(1.f);
    noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise1.SetFractalOctaves(1);
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
            chunks[{ x, z}] = emptychunk;
            chunks[{ x, z}].generateChunk(noise, noise2, chunkSize, chunkHeight, x * chunkSize, z * chunkSize);
            chunks[{ x, z}].generateVAO();
        }
    }

    test();


    for (int x{ 0 - renderDistance }; x < renderDistance; x++) {
        for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
            //chunks[{x, z}].generateVertices();
            chunks[{x, z}].bindVerticesToVao();
        }
    }


    return true; // testing something revert if not working
}

void World::render() {

    for (auto i{ chunksTORenderVao.begin() }; i != chunksTORenderVao.end(); i++) {
        if (i->second) {
            i->second->generateVAO();
            i->second->bindVerticesToVao();
            i->second->notifyNeighbours();
            i->second = nullptr;
        }
    }

    for (auto i{ chunks.begin() }; i != chunks.end(); i++) {
        if (i->second.needRebindVertices && i->second.vaoMade)
            i->second.bindVerticesToVao();
        i->second.render();
    }
}



void World::loadNewChunks(glm::vec3 playerposition)
{
    int playerX = std::floor(playerposition.x / 16.f);
    int playerZ = std::floor(playerposition.z / 16.f);

    static std::map<ChunkPosition, Chunk*> NewChunks;

    constexpr int playerRDistance = 8;

    double timer = glfwGetTime();


    for (int x{ playerX - playerRDistance }; x <= playerX + playerRDistance; x++) {
        for (int z{ playerZ - playerRDistance }; z <= playerZ + playerRDistance; z++) {
            if (chunks.find({ x, z}) == chunks.end()) {
                chunks[{ x, z }] = Chunk();
                chunks[{ x, z }].generateChunk(noise, noise2, chunkSize, chunkHeight, x * chunkSize, z * chunkSize);
                
                NewChunks[{ x, z }] = &chunks[{ x, z }];
            }
        }
    }
    if (NewChunks.empty())
        return;

    double timepassed = glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :noisemap chunk generation:%f\n", timepassed);

    for (auto i{ NewChunks.begin() }; i != NewChunks.end(); i++) {
        updateChunkNeighbours(i->first);
    }
    timepassed = glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :generating chunk mesh:%f\n", timepassed);

    for (auto i{ NewChunks.begin() }; i != NewChunks.end(); i++) {
        NewChunks[i->first]->notifyNeighbours();
        chunksTORenderVao[i->first] = &chunks[i->first];
    }

    timepassed = glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :Notifying neighbours:%f\n\n\n", timepassed);

    NewChunks.clear();
}
