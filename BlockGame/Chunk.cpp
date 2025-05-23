#include "Chunk.h"
#include "PerlinNoise.h"
#include <iostream>    



void generateNoiseMap(std::vector<int>& chunk, int chunksize, int chunkheight, int startX, int startZ) {

    chunk.clear();
    chunk.reserve(chunksize * chunksize);


    int GRID_SIZE = 200;
    float highest = 0.f;
    float lowest = 0.f;

    for (int x{ 0 }; x < chunksize; x++) {
        for (int z{ 0 }; z < chunksize; z++) {

            float val = 0.f;

            float freq = 1.f;
            float amp = 1.f;


            for (int i{ 0 }; i < 4; i++) {
                val += PerlinNoise::perlin((x + startX) * freq / GRID_SIZE, (z + startZ) * freq / GRID_SIZE) * amp;

                freq *= 2.f;
                amp /= 2.f;
            }

            val *= 1.2f;

            val += 1.f;
            val *= .5f;
            // turn value from -1 to 1 into 0-1

            val *= chunkheight;
            chunk.push_back((int)val);

            //std::cout << val << '\n';
            if (val > highest)
                highest = val;
            if (val < lowest)
                lowest = val;
        }
    }
}



float clamp(float x, float lowerlimit = 0.0f, float upperlimit = 1.0f) {
    if (x < lowerlimit) return lowerlimit;
    if (x > upperlimit) return upperlimit;
    return x;
}

float smoothstep(float edge0, float edge1, float x) {
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0));

    return x * x * (3.0f - 2.0f * x);
}


inline void generateFastNoiseMap(const FastNoiseLite& noise, std::vector<int>& chunkheightvalues, int chunksize, int xoffset, int zoffset, float powerofnoise) {

    
    int groundstart = 100;
    int highestground = 170;
    int groundheight = highestground - groundstart;

    for (int x{ 0 }; x < chunksize; x++) {
        for (int z{ 0 }; z < chunksize; z++) {


            float val = noise.GetNoise((float)(x + xoffset), (float)(z + zoffset));

            val += 1.f;
            val *= .5f;
            val = smoothstep(0.f, 1.f, val);
            val *= groundheight;
            val += groundstart;
        


            /* 
            float noise2val = noise2.GetNoise((float)(x + xoffset), (float)(z + zoffset));
            noise2val += 1.f;
            noise2val *= .5f;
            noise2val *= chunksize;

            val *= 0.8f;
            val += 0.2f * noise2val;
            */
            chunkheightvalues[chunksize * x + z] += (int)(val * powerofnoise);
        }
    }

}




void Chunk::generateChunk(const FastNoiseLite& noise, const FastNoiseLite& noise2, int _chunksize, int _chunkheight, int _xoffset, int _zoffset)
{

    chunksize = _chunksize;
    chunkheight = _chunkheight;

    chunkheight = 256;


    xoffset = _xoffset;
    zoffset = _zoffset;

    std::vector<int> chunkheightvalues(chunksize * chunksize,0);
    generateFastNoiseMap(noise, chunkheightvalues, chunksize, xoffset, zoffset, 0.7f);
    generateFastNoiseMap(noise2, chunkheightvalues, chunksize, xoffset, zoffset,0.3f);
    cubes = std::vector<Cube>(chunksize * chunkheight * chunksize,0); // x,y,z
    


    // generate chunk 
    for (int x{ 0 }; x < chunksize; x++) {
        for (int y{ 0 }; y < chunkheight; y++) {
            for (int z{ 0 }; z < chunksize; z++) {
                //Cube& cube = cubes[(x * chunksize * chunkheight) + y * chunksize + z];

                if (y < chunkheightvalues[x * chunksize + z])
                    cubes[(x * chunksize * chunkheight) + y * chunksize + z].setId(1);
                else if (y == chunkheightvalues[x * chunksize + z])
                    cubes[(x * chunksize * chunkheight) + y * chunksize + z].setId(2);

                //cube.setPosition(x + xoffset, y, z + zoffset);
            }
        }
    }
    // generate visible vertices

}

void Chunk::generateVAO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vaoMade = true;
}

void Chunk::bindVerticesToVao()
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * visiblevertices.size(), &visiblevertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // stored on gpu
    amountOfVertices = visiblevertices.size();
    visiblevertices.clear();

    needRebindVertices = false;
}


void Chunk::generateVertices()
{
    needRebindVertices = false; 
    visiblevertices.clear();
    // set to false so we don't rebind vertices while mid generation
    // for example if we just generate vertice and neighbours get updated
    // we would try to generate again
    // 
    // very bad code tbh. will revisit when comparing walls to neighbouring chunks

    glm::ivec3 tempCubePos{};
    glm::ivec3 worldCubepos{};
    for (int x{ 0 }; x < chunksize; x++) {
        for (int y{ 0 }; y < chunkheight; y++) {
            for (int z{ 0 }; z < chunksize; z++) {
                Cube& cube = this->cubes[(x * chunksize * chunkheight) + (y * chunksize) + z];
                if (cube.isAir())
                    continue;
                tempCubePos.x = x;
                tempCubePos.y = y;
                tempCubePos.z = z;
                worldCubepos.x = x + xoffset;
                worldCubepos.y = y;
                worldCubepos.z = z + zoffset;
                // check all directions to see if we are next to air block
                // if we are we need to draw that sprite
                if (tempCubePos.y + 1 == chunkheight) // always add blocks on the border of the chunk ?!?!?
                    cube.addTopVertexData(visiblevertices, worldCubepos);
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + ((tempCubePos.y + 1) * chunksize) + tempCubePos.z].isAir()) {
                    cube.addTopVertexData(visiblevertices, worldCubepos);
                }
                if (tempCubePos.y == 0) {
                    //cube.addBottomVertexData(visiblevertices, worldCubepos);
                }
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + ((tempCubePos.y - 1) * chunksize) + tempCubePos.z].isAir()) {
                    cube.addBottomVertexData(visiblevertices, worldCubepos);
                }

                if (tempCubePos.x == 0) { // left check
                    if (neighbours.left) {
                        if (neighbours.left->getBlockId({ chunksize-1,y,z }) == 0) { // if block of air(id = 0)
                            cube.addLeftVertexData(visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addLeftVertexData(visiblevertices, worldCubepos);
                    }
                }
                else if (cubes[((tempCubePos.x - 1) * chunksize * chunkheight) + (tempCubePos.y * chunksize) + tempCubePos.z].isAir()) {
                    cube.addLeftVertexData(visiblevertices, worldCubepos);
                }


                if (tempCubePos.x + 1 == chunksize) { // right check
                    if (neighbours.right) {
                        if (neighbours.right->getBlockId({ 0,y,z }) == 0) { // if block of air(id = 0)
                            cube.addRightVertexData(visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addRightVertexData(visiblevertices, worldCubepos);
                    }
                }
                else if (cubes[((tempCubePos.x + 1) * chunksize * chunkheight) + (tempCubePos.y * chunksize) + tempCubePos.z].isAir()) {
                    cube.addRightVertexData(visiblevertices, worldCubepos);
                }


                if (tempCubePos.z == 0) { // backwards check
                    if (neighbours.back) {
                        if (neighbours.back->getBlockId({ x,y,chunksize-1 }) == 0) { // if block of air(id = 0)
                            cube.addBackwardVertexData(visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addBackwardVertexData(visiblevertices, worldCubepos);
                    }
                }
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + (tempCubePos.y * chunksize) + (tempCubePos.z - 1)].isAir()) {
                    cube.addBackwardVertexData(visiblevertices, worldCubepos);
                }



                if (tempCubePos.z + 1 == chunksize) { // forward check
                    if (neighbours.front) {
                        if (neighbours.front->getBlockId({ x,y,0 }) == 0) { // if block of air(id = 0)
                            cube.addForwardVertexData(visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addForwardVertexData(visiblevertices, worldCubepos);
                    }
                }
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + (tempCubePos.y * chunksize) + (tempCubePos.z + 1)].isAir()) {
                    cube.addForwardVertexData(visiblevertices, worldCubepos);
                }
            }
        }
    }
    needRebindVertices = true;
}

void Chunk::render()
{
    //printf("Amountofevertices:%i\n", amountOfVertices);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, amountOfVertices);
    glBindVertexArray(0);

}

inline bool operator==(const ChunkNeighbors& lhs, const ChunkNeighbors& rhs) {

    if (lhs.back != rhs.back)
        return false;
    if (lhs.left != rhs.left)
        return false;
    if (lhs.right!= rhs.right)
        return false;
    if (lhs.front != rhs.front)
        return false;
    return true;
}

// MAKE SURE TO UPDATE WHEN UNLOADING CHUNKS
// THREADING :THUMBSUP:
// ONLY CALL THIS ON MAIN THREAD PRETTY PLEASE
void Chunk::setNeighbours(const ChunkNeighbors& _neighbours)
{
    if (this->neighbours == _neighbours)
        return;
    

    this->neighbours = _neighbours;

    amountOfVertices = 0;
    generateVertices();

    //static int callers = 0;
    //callers++;
    //printf("Setting neighbours %i\n", callers);
}

void Chunk::notifyNeighbours()
{
    if (neighbours.front) {
        neighbours.front->updateNeighbour(NeighbourDirection::BACK, this);
    }
    if (neighbours.back) {
        neighbours.back->updateNeighbour(NeighbourDirection::FRONT, this);
    }
    if (neighbours.right) {
        neighbours.right->updateNeighbour(NeighbourDirection::LEFT, this);
    }
    if (neighbours.left) {
        neighbours.left->updateNeighbour(NeighbourDirection::RIGHT, this);
    }
}

void Chunk::updateNeighbour(NeighbourDirection direction, Chunk* newNeighbour)
{
    switch (direction) {
    case NeighbourDirection::FRONT:
        if (neighbours.front == newNeighbour)
            return;
        neighbours.front = newNeighbour;
        break;
    case NeighbourDirection::BACK:
        if (neighbours.back == newNeighbour)
            return;
        neighbours.back = newNeighbour;
        break;
    case NeighbourDirection::LEFT:
        if (neighbours.left == newNeighbour)
            return;
        neighbours.left = newNeighbour;
        break;
    case NeighbourDirection::RIGHT:
        if (neighbours.right == newNeighbour)
            return;
        neighbours.right = newNeighbour;
        break;
    }
    //printf("Updating neighbours %i\n", (int)visiblevertices.size());
    generateVertices();
}

int Chunk::getBlockId(const glm::ivec3& pos)
{
    if (cubes.empty()) {
        printf("GetBlockId We have no cubes\n");
        return 0;
    }
    return cubes[(pos.x * chunksize * chunkheight) + (pos.y * chunksize) + pos.z].getId();

}


//std::vector<vertex>& chunk::getvisiblevertices()
//{
//    return visiblevertices;
//}
