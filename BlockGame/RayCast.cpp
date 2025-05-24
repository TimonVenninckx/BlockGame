#include "RayCast.h"


std::vector<Vertex> RayCast::rays;


RayCast::RayCast(glm::vec3 startPos, glm::vec3 direction, float rayLength)
    : m_StartPos{ startPos }
    , m_Direction{ direction }
    , m_RayLength{rayLength}

{
    m_Direction = glm::normalize(m_Direction);

    rays.push_back(Vertex{ startPos,{1.f,1.f,1.f},{1.f,1.f,1.f} });
    rays.push_back(Vertex{ startPos + direction * rayLength,{1.f,1.f,1.f},{1.f,1.f,1.f} });

    printf("\n\n\nNew raycast\n\n");
    printf("ray ,%f,%f,%f\n", startPos.x, startPos.y, startPos.z);
    printf("ray ,%f,%f,%f\n", direction.x * rayLength, direction.y * rayLength, direction.z * rayLength);
}


//  TODO CHECK IF CHUNK WE MOVE TO EXISTS
glm::ivec3 RayCast::CheckForBlock(World & world) {

    // goated article
    // http://www.cse.yorku.ca/~amana/research/grid.pdf
    // http://www.cse.yorku.ca/~amana/research/grid.pdf
    // http://www.cse.yorku.ca/~amana/research/grid.pdf

    // just spend 20m onthis fuckoff
    //int X = std::lround(m_StartPos.x) % world.ChunkSize;
    //int Y = std::lround(m_StartPos.y);
    //int Z = std::lround(m_StartPos.z) & world.ChunkSize;

    // localize x,y,z to in chunk coordinates
    int X = std::lround(m_StartPos.x) % world.ChunkSize;
    int Y = std::lround(m_StartPos.y);
    int Z = std::lround(m_StartPos.z) % world.ChunkSize;

    if (X < 0)
        X += world.ChunkSize;
    if (Z < 0)
        Z += world.ChunkSize;

    printf("RayBlockStartingPosition: %i,%i,%i\n", X, Y, Z);


    ChunkPosition currentChunk = { std::lround(std::floor(std::lround(m_StartPos.x) / (float)world.ChunkSize)), std::lround(std::floor(std::lround(m_StartPos.z) / (float)world.ChunkSize)) };


    int stepX = m_Direction.x >= 0 ? 1 : -1;
    int stepY = m_Direction.y >= 0 ? 1 : -1;
    int stepZ = m_Direction.z >= 0 ? 1 : -1;

    // max raylength block + 1 in local values
    int justOutX = std::lround(m_Direction.x * m_RayLength + std::fmod(m_StartPos.x, world.ChunkSize)) + stepX;
    int justOutY = std::lround(m_Direction.y * m_RayLength + m_StartPos.y) + stepY;
    int justOutZ = std::lround(m_Direction.z * m_RayLength + std::fmod(m_StartPos.z, world.ChunkSize)) + stepZ;

    printf("Justoutx:%i\n", justOutX);
    printf("Justoutz:%i\n", justOutZ);
    printf("Currentchunk %i,%i\n\n", currentChunk.first, currentChunk.second);

    // calculate as inverse first
    float invVX = m_Direction.x == 0 ? std::numeric_limits<double>::infinity() : 1.f / m_Direction.x;
    float invVY = m_Direction.y == 0 ? std::numeric_limits<double>::infinity() : 1.f / m_Direction.y;
    float invVZ = m_Direction.z == 0 ? std::numeric_limits<double>::infinity() : 1.f / m_Direction.z;

    float tMaxX{};
    float tMaxY{};
    float tMaxZ{};


    float tDeltaX = std::abs(invVX);
    float tDeltaY = std::abs(invVY);
    float tDeltaZ = std::abs(invVZ);

    if (stepX > 0) {
        tMaxX = (1.f - std::fmod(m_StartPos.x, 1.f)) * tDeltaX;
    }
    else {
        tMaxX = (1.f + std::fmod(m_StartPos.x, 1.f)) * tDeltaX;
    }

    if (stepY > 0) {
        tMaxY = (1.f - std::fmod(m_StartPos.y, 1.f)) * tDeltaY;
    }
    else {
        tMaxY = (1.f + std::fmod(m_StartPos.y, 1.f)) * tDeltaY;
    }

    if (stepZ > 0) {
        tMaxZ = (1.f - std::fmod(m_StartPos.z, 1.f)) * tDeltaZ;
    }
    else {
        tMaxZ = (1.f + std::fmod(m_StartPos.z, 1.f)) * tDeltaZ;
    }
     //= ((float)(X + 1) - m_StartPos.x) / m_Direction.x * m_RayLength;
     //= ((float)(Y + 1) - m_StartPos.y) / m_Direction.y * m_RayLength;
     //= ((float)(Z + 1) - m_StartPos.x) / m_Direction.z * m_RayLength;

    


        // value along ray that says we are in the next block

    /*printf("StartPosition: %i,%i,%i\n", X, Y, Z);
    printf("Tmax Position: %i,%i,%i\n", tMaxX, tMaxY, tMaxZ);*/
    
    


    while (true) {

        // first check the block we reside in
        int blockId = world.getChunks()[currentChunk].GetBlockId({ X,Y,Z });

        printf("Chunk id at:{%i,%i}\n", currentChunk.first, currentChunk.second);
        printf("Block id at:{%i,%i,%i} = %i\n", X, Y, Z, blockId);

        if (blockId) { // not air
            world.getChunks()[currentChunk].RemoveBlock({ X,Y,Z });
            return {};
        }


        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                X = X + stepX;
                if (X == justOutX)
                    return {}; /* outside grid */
                tMaxX = tMaxX + tDeltaX;
            }
            else {
                Z = Z + stepZ;
                if (Z == justOutZ)
                    return {};
                tMaxZ = tMaxZ + tDeltaZ;
            }
        }
        else {
            if (tMaxY < tMaxZ) {
                Y = Y + stepY;
                if (Y == justOutY)
                    return {};
                tMaxY = tMaxY + tDeltaY;
            }
            else {
                Z = Z + stepZ;
                if (Z == justOutZ)
                    return {};
                tMaxZ = tMaxZ + tDeltaZ;
            }
        }

        // if number becomes negative move onto next chunk
        if (X < 0) { // chunk left
            X = world.ChunkSize - 1;
            justOutX = justOutX + world.ChunkSize;
            currentChunk.first--;// decrent chunk
        }
        else if (X >= world.ChunkSize) { // chunk to right
            X = 0;
            justOutX = justOutX - world.ChunkSize; // fix this
            currentChunk.first++;
        }
        if (Z < 0) { // chunk backward
            Z = world.ChunkSize - 1;
            justOutZ = justOutZ + world.ChunkSize;
            currentChunk.second--;// decrent chunk
        }
        else if (Z >= world.ChunkSize) { // chunk forward
            Z = 0;
            printf("JustOutZ: %i\n", justOutZ);
            justOutZ = justOutZ - world.ChunkSize;
            printf("JustOutZ: %i\n", justOutZ);
            currentChunk.second++;
        }
    }
        
    
    return glm::ivec3(1, 1, 1);
}
