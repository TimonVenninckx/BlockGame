#include "Chunk.h"
#include <iostream>    




float clamp(float x, float lowerLimit = 0.0f, float upperLimit = 1.0f) {
    if (x < lowerLimit) return lowerLimit;
    if (x > upperLimit) return upperLimit;
    return x;
}

float smoothstep(float edge0, float edge1, float x) {
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0));

    return x * x * (3.0f - 2.0f * x);
}


inline void generateFastNoiseMap(const FastNoiseLite& noise, std::vector<int>& chunkHeightValues, int chunkSize, int xOffset, int zOffset, float powerOfNoise) {

    
    int groundstart = 100;
    int highestground = 150;
    int groundheight = highestground - groundstart;

    for (int x{ 0 }; x < chunkSize; x++) {
        for (int z{ 0 }; z < chunkSize; z++) {


            float val = noise.GetNoise((float)(x + xOffset), (float)(z + zOffset));

            val += 1.f;
            val *= .5f;
            val = smoothstep(0.f, 1.f, val);
            val *= groundheight;
            val += groundstart;
        
            chunkHeightValues[chunkSize * x + z] += (int)(val * powerOfNoise);
        }
    }

}




void Chunk::GenerateChunk(const FastNoiseLite& noise, const FastNoiseLite& noise2, int chunkSize, int chunkHeight, int xOffset, int zOffset)
{

    m_chunksize = chunkSize;
    m_chunkheight = chunkHeight;


    m_xoffset = xOffset;
    m_zoffset = zOffset;

    std::vector<int> chunkheightvalues(m_chunksize * m_chunksize, 0);
    generateFastNoiseMap(noise, chunkheightvalues, m_chunksize, m_xoffset, m_zoffset, 0.8f);
    generateFastNoiseMap(noise2, chunkheightvalues, m_chunksize, m_xoffset, m_zoffset, 0.2f);
    m_cubes = std::vector<Cube>(m_chunksize * m_chunkheight * m_chunksize,0); // x,y,z
    


    // generate chunk 
    for (int x{ 0 }; x < m_chunksize; x++) {
        for (int y{ 0 }; y < m_chunkheight; y++) {
            for (int z{ 0 }; z < m_chunksize; z++) {
                //Cube& cube = cubes[(x * chunksize * chunkheight) + y * chunksize + z];

                if (y < chunkheightvalues[x * m_chunksize + z])
                    m_cubes[(x * m_chunksize * m_chunkheight) + y * m_chunksize + z].setId(1);
                else if (y == chunkheightvalues[x * m_chunksize + z])
                    m_cubes[(x * m_chunksize * m_chunkheight) + y * m_chunksize + z].setId(2);

                //cube.setPosition(x + xoffset, y, z + zoffset);
            }
        }
    }
    // generate visible vertices

}

void Chunk::GenerateVAO()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBindVertexArray(0);
    m_vaoMade = true;
}

void Chunk::BindVerticesToVao()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_visiblevertices.size(), &m_visiblevertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // stored on gpu
    m_amountOfVertices = m_visiblevertices.size();
    m_visiblevertices.clear();

    m_needRebindVertices = false;
}


void Chunk::GenerateVertices()
{
    m_needRebindVertices = false;
    m_visiblevertices.clear();
    // set to false so we don't rebind vertices while mid generation
    // for example if we just generate vertice and neighbours get updated
    // we would try to generate again
    // 
    // very bad code tbh. will revisit when comparing walls to neighbouring chunks

    glm::ivec3 tempCubePos{};
    glm::ivec3 worldCubepos{};
    for (int x{ 0 }; x < m_chunksize; x++) {
        for (int y{ 0 }; y < m_chunkheight; y++) {
            for (int z{ 0 }; z < m_chunksize; z++) {
                int currentIndex = (x * m_chunksize * m_chunkheight) + (y * m_chunksize) + z;
                Cube& cube = m_cubes[currentIndex];
                if (cube.isAir())
                    continue;
                tempCubePos.x = x;
                tempCubePos.y = y;
                tempCubePos.z = z;
                worldCubepos.x = x + m_xoffset;
                worldCubepos.y = y;
                worldCubepos.z = z + m_zoffset;
                // check all directions to see if we are next to air block
                // if we are we need to draw that sprite
                if (tempCubePos.y + 1 == m_chunkheight) // always add blocks on the border of the chunk ?!?!?
                    cube.addTopVertexData(m_visiblevertices, worldCubepos);
                else if (m_cubes[currentIndex + 1 * m_chunksize].isAir()) {
                    cube.addTopVertexData(m_visiblevertices, worldCubepos);
                }
                if (tempCubePos.y == 0) {
                    //cube.addBottomVertexData(visiblevertices, worldCubepos);
                }
                else if (m_cubes[currentIndex - 1 * m_chunksize].isAir()) { // accesing y
                    cube.addBottomVertexData(m_visiblevertices, worldCubepos);
                }

                if (tempCubePos.x == 0) { // left check
                    if (m_neighbours.left) {
                        if (m_neighbours.left->GetBlockId({ m_chunksize-1,y,z }) == 0) { // if block of air(id = 0)
                            cube.addLeftVertexData(m_visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addLeftVertexData(m_visiblevertices, worldCubepos);
                    }
                }
                else if (m_cubes[currentIndex - 1 * m_chunksize * m_chunkheight].isAir()) {
                    cube.addLeftVertexData(m_visiblevertices, worldCubepos);
                }


                if (tempCubePos.x + 1 == m_chunksize) { // right check
                    if (m_neighbours.right) {
                        if (m_neighbours.right->GetBlockId({ 0,y,z }) == 0) { // if block of air(id = 0)
                            cube.addRightVertexData(m_visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addRightVertexData(m_visiblevertices, worldCubepos);
                    }
                }
                else if (m_cubes[currentIndex + 1 * m_chunksize * m_chunkheight].isAir()) {
                    cube.addRightVertexData(m_visiblevertices, worldCubepos);
                }


                if (tempCubePos.z == 0) { // backwards check
                    if (m_neighbours.back) {
                        if (m_neighbours.back->GetBlockId({ x,y,m_chunksize-1 }) == 0) { // if block of air(id = 0)
                            cube.addBackwardVertexData(m_visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addBackwardVertexData(m_visiblevertices, worldCubepos);
                    }
                }
                else if (m_cubes[currentIndex - 1].isAir()) {
                    cube.addBackwardVertexData(m_visiblevertices, worldCubepos);
                }



                if (tempCubePos.z + 1 == m_chunksize) { // forward check
                    if (m_neighbours.front) {
                        if (m_neighbours.front->GetBlockId({ x,y,0 }) == 0) { // if block of air(id = 0)
                            cube.addForwardVertexData(m_visiblevertices, worldCubepos);
                        }
                    }
                    else {
                        cube.addForwardVertexData(m_visiblevertices, worldCubepos);
                    }
                }
                else if (m_cubes[currentIndex + 1].isAir()) {
                    cube.addForwardVertexData(m_visiblevertices, worldCubepos);
                }
            }
        }
    }
    m_needRebindVertices = true;
}

void Chunk::Render()
{
    //printf("Amountofevertices:%i\n", amountOfVertices);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);
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
void Chunk::SetNeighbours(const ChunkNeighbors& neighBours)
{
    if (m_neighbours == neighBours)
        return;
    

    m_neighbours = neighBours;

    m_amountOfVertices = 0;
    GenerateVertices();

    //static int callers = 0;
    //callers++;
    //printf("Setting neighbours %i\n", callers);
}

void Chunk::NotifyNeighbours()
{
    if (m_neighbours.front) {
        m_neighbours.front->UpdateNeighbour(ENeighbourDirection_BACK, this);
    }
    if (m_neighbours.back) {
        m_neighbours.back->UpdateNeighbour(ENeighbourDirection_FRONT, this);
    }
    if (m_neighbours.right) {
        m_neighbours.right->UpdateNeighbour(ENeighbourDirection_LEFT, this);
    }
    if (m_neighbours.left) {
        m_neighbours.left->UpdateNeighbour(ENeighbourDirection_RIGHT, this);
    }
}

void Chunk::UpdateNeighbour(ENeighbourDirection direction, Chunk* newNeighbour)
{
    switch (direction) {
    case ENeighbourDirection_FRONT:
        if (m_neighbours.front == newNeighbour)
            return;
        m_neighbours.front = newNeighbour;
        break;
    case ENeighbourDirection_BACK:
        if (m_neighbours.back == newNeighbour)
            return;
        m_neighbours.back = newNeighbour;
        break;
    case ENeighbourDirection_LEFT:
        if (m_neighbours.left == newNeighbour)
            return;
        m_neighbours.left = newNeighbour;
        break;
    case ENeighbourDirection_RIGHT:
        if (m_neighbours.right == newNeighbour)
            return;
        m_neighbours.right = newNeighbour;
        break;
    }
    //printf("Updating neighbours %i\n", (int)visiblevertices.size());
    GenerateVertices();
}

int Chunk::GetBlockId(const glm::ivec3& pos)
{
    if (m_cubes.empty()) {
        printf("GetBlockId We have no cubes\n");
        return 0;
    }
    return m_cubes[(pos.x * m_chunksize * m_chunkheight) + (pos.y * m_chunksize) + pos.z].getId();

}

void Chunk::RemoveBlock(const glm::ivec3& pos)
{
    if (m_cubes.empty()) {
        printf("GetBlockId We have no cubes\n");
        return ;
    }
    m_cubes[(pos.x * m_chunksize * m_chunkheight) + (pos.y * m_chunksize) + pos.z].setId(0);
    GenerateVertices();

    
    if (pos.z == 0                      && m_neighbours.back) {
        m_neighbours.back->GenerateVertices();
    }
    else if (pos.z == m_chunksize - 1   && m_neighbours.front) {
        m_neighbours.front->GenerateVertices();
    }
    if (pos.x == 0                      && m_neighbours.left) {
        m_neighbours.left->GenerateVertices();
    }
    else if (pos.x == m_chunksize - 1   && m_neighbours.right) {
        m_neighbours.right->GenerateVertices();
    }
}

bool Chunk::NeedRebindVertices() const
{
    return m_needRebindVertices;
}

bool Chunk::VaoMade() const
{
    return m_vaoMade;
}


//std::vector<vertex>& chunk::getvisiblevertices()
//{
//    return visiblevertices;
//}
