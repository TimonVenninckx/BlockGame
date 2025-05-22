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


void generateFastNoiseMap(const FastNoiseLite& noise, std::vector<int>& chunkheightvalues, int chunksize, int xoffset, int zoffset, float powerofnoise) {

    
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
    

    // generate chunk 
    for (int x{ 0 }; x < chunksize; x++) {
        for (int y{ 0 }; y < chunkheight; y++) {
            for (int z{ 0 }; z < chunksize; z++) {
                Cube cube;


                if (y < chunkheightvalues[x * chunksize + z])
                    cube.setId(1);
                else if (y == chunkheightvalues[x * chunksize + z])
                    cube.setId(2);

                //cube.setPosition(x + xoffset, y, z + zoffset);
                this->cubes.push_back(cube);
            }
        }
    }
    // generate visible vertices

}

void Chunk::generateVAO()
{

    //printf("WE HAVE %i vertices to render\n",visiblevertices.size());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
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
}

void Chunk::generateVertices()
{
    // bad
    // bad
    // bad
    // bad

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
                    //cube.addLeftVertexData(visiblevertices, worldCubepos);
                }
                else if (cubes[((tempCubePos.x - 1) * chunksize * chunkheight) + (tempCubePos.y * chunksize) + tempCubePos.z].isAir()) {
                    cube.addLeftVertexData(visiblevertices, worldCubepos);
                }
                if (tempCubePos.x + 1 == chunksize) { // right check
                    //cube.addRightVertexData(visiblevertices, worldCubepos);
                }
                else if (cubes[((tempCubePos.x + 1) * chunksize * chunkheight) + (tempCubePos.y * chunksize) + tempCubePos.z].isAir()) {
                    cube.addRightVertexData(visiblevertices, worldCubepos);
                }

                if (tempCubePos.z == 0) { // backwards check
                    //cube.addBackwardVertexData(visiblevertices, worldCubepos);
                }
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + (tempCubePos.y * chunksize) + (tempCubePos.z - 1)].isAir()) {
                    cube.addBackwardVertexData(visiblevertices, worldCubepos);
                }
                if (tempCubePos.z + 1 == chunksize) { // forward check
                    //cube.addForwardVertexData(visiblevertices, worldCubepos);
                }
                else if (cubes[(tempCubePos.x * chunksize * chunkheight) + (tempCubePos.y * chunksize) + (tempCubePos.z + 1)].isAir()) {
                    cube.addForwardVertexData(visiblevertices, worldCubepos);
                }
            }
        }
    }
}

void Chunk::render()
{

    //printf("i have %i vertices\n", amountOfVertices);

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

// THREADING :THUMBSUP:
// ONLY CALL THIS ON MAIN THREAD PRETTY PLEASE
void Chunk::setNeighbours(const ChunkNeighbors& _neighbours)
{
    if (this->neighbours == _neighbours)
        return;

    amountOfVertices = 0;
    generateVertices();

    // rest vertices


    glGenVertexArrays(1, &vao);
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

}


//std::vector<vertex>& chunk::getvisiblevertices()
//{
//    return visiblevertices;
//}
