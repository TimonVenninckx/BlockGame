#include "World.h"
#include <iostream>
#include <GLFW/glfw3.h>

constexpr int renderDistance = 4;

const int World::ChunkSize{ 16 };
const int World::ChunkHeight{ 256 };


void World::Test()
{
    for (auto i{ m_Chunks.begin() }; i != m_Chunks.end(); i++) {
        UpdateChunkNeighbours(i->first);
    }
}

void World::UpdateChunkNeighbours(ChunkPosition chunkLocation)
{
    if (m_Chunks.find(chunkLocation) == m_Chunks.end()) {
        printf("Could not find chunk :: updateChunkNeighbours\n");
        return;
    }

    //printf("Position = %i:%i\n", chunkLocation.first, chunkLocation.second);

    ChunkNeighbors neighBours;

    ChunkPosition pos = chunkLocation;
    pos = chunkLocation + ChunkPosition{ 0,-1 }; // backwards
    if (m_Chunks.find(pos) == m_Chunks.end()) {
        neighBours.back = nullptr;
    }
    else {
        neighBours.back = &m_Chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ 0,1 }; // forwards
    if (m_Chunks.find(pos) == m_Chunks.end()) {
        neighBours.front = nullptr;
    }
    else {
        neighBours.front = &m_Chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ 1,0 }; // right
    if (m_Chunks.find(pos) == m_Chunks.end()) {
        neighBours.right = nullptr;
    }
    else {
        neighBours.right = &m_Chunks[pos];
    }

    pos = chunkLocation + ChunkPosition{ -1,0 }; // left
    if (m_Chunks.find(pos) == m_Chunks.end()) {
        neighBours.left = nullptr;
    }
    else {
        neighBours.left = &m_Chunks[pos];
    }
    m_Chunks[chunkLocation].SetNeighbours(neighBours);
}

bool World::GenerateWorld(int seed)
{
    m_Noise.SetSeed(seed);
    m_Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_Noise.SetFractalLacunarity(1.6f);
    m_Noise.SetFrequency(.007f);
    m_Noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_Noise.SetFractalOctaves(5);
    m_Noise.SetFractalGain(1.0f);
    m_Noise.SetFractalWeightedStrength(2.2f);


    m_Noise1.SetSeed(seed);
    m_Noise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_Noise1.SetFrequency(0.04);
    m_Noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_Noise1.SetFractalOctaves(5);
    m_Noise1.SetFractalGain(.2f);
    m_Noise1.SetFractalWeightedStrength(.0f);


    m_Noise2.SetSeed(seed);
    m_Noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_Noise2.SetFrequency(.006f);
    m_Noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_Noise2.SetFractalOctaves(5);
    m_Noise2.SetFractalGain(.3f);
    m_Noise2.SetFractalWeightedStrength(.0f);




    //Chunk emptychunk;
    //for (int x{ 0 - renderDistance}; x < renderDistance; x++) {
    //    for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
    //        chunks[{ x, z}] = emptychunk;
    //        chunks[{ x, z}].generateChunk(noise, noise2, chunkSize, chunkHeight, x * chunkSize, z * chunkSize);
    //        chunks[{ x, z}].generateVAO();
    //    }
    //}

    //test();


    //for (int x{ 0 - renderDistance }; x < renderDistance; x++) {
    //    for (int z{ 0 - renderDistance }; z < renderDistance; z++) {
    //        //chunks[{x, z}].GenerateVertices();
    //        chunks[{x, z}].bindVerticesToVao();
    //    }
    //} 

    return true; // testing something revert if not working
}

void World::Render() {

    for (auto i{ m_ChunksToRenderVao.begin() }; i != m_ChunksToRenderVao.end(); i++) {
        if (i->second) {
            i->second->GenerateVAO();
            i->second->BindVerticesToVao();
            i->second->NotifyNeighbours();
            i->second = nullptr;
        }
    }

    for (auto i{ m_Chunks.begin() }; i != m_Chunks.end(); i++) {
        if (i->second.NeedRebindVertices() && i->second.VaoMade())
            i->second.BindVerticesToVao();
        i->second.Render();
    }
}

 std::map<ChunkPosition, Chunk>& World::getChunks()
{
    return m_Chunks;
}



void World::LoadNewChunks(glm::vec3 playerPosition)
{
    int playerX = std::floor(playerPosition.x / 16.f);
    int playerZ = std::floor(playerPosition.z / 16.f);

    static std::map<ChunkPosition, Chunk*> NewChunks;


    double timer = glfwGetTime();


    for (int x{ playerX - renderDistance }; x <= playerX + renderDistance; x++) {
        for (int z{ playerZ - renderDistance }; z <= playerZ + renderDistance; z++) {
            if (m_Chunks.find({ x, z}) == m_Chunks.end()) {
                m_Chunks[{ x, z }] = Chunk();
                m_Chunks[{ x, z }].GenerateChunk(m_Noise, m_Noise2, ChunkSize, ChunkHeight, x * ChunkSize, z * ChunkSize);
                
                NewChunks[{ x, z }] = &m_Chunks[{ x, z }];
            }
        }
    }
    if (NewChunks.empty())
        return;


    /*double chunkgentimepassed = glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :noisemap chunk generation:%f\n", chunkgentimepassed);*/

    for (auto i{ NewChunks.begin() }; i != NewChunks.end(); i++) {
        UpdateChunkNeighbours(i->first);
    }
    /*double genmeshtime = glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :generating chunk mesh:%f\n", genmeshtime);*/

    for (auto i{ NewChunks.begin() }; i != NewChunks.end(); i++) {
        NewChunks[i->first]->NotifyNeighbours();
        m_ChunksToRenderVao[i->first] = &m_Chunks[i->first];
    }

    /*double notifyneighbourstimepassed= glfwGetTime() - timer;
    timer = glfwGetTime();
    printf("Time passed :Notifying neighbours:%f\n", notifyneighbourstimepassed);

    printf("Time passed genmesh / chunkgen:%f\n\n\n", genmeshtime / chunkgentimepassed);*/
    //printf("Time passed :Notifying neighbours:%f\n\n\n", timepassed);


    NewChunks.clear();
}
