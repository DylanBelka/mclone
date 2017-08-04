#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <memory>
#include <iostream>
#include <random>

#include <chrono>
#include <mutex>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/noise.hpp>

#include "Block.h"
#include "Shader.h"
#include "math.h"

const unsigned int chunkWidth = 16;
const unsigned int chunkHeight = 256;
const unsigned int chunkDepth = 16;

const unsigned int chunkSize = chunkWidth * chunkDepth * chunkHeight;

/****

	A chunk holds 16 * 16 * 256 blocks 
	Handles converting Blocks in chunk into a single chunk mesh
	For faster drawing + fewer draw calls
	
****/

class Chunk
{
public:
	friend class World;

	Chunk();
	~Chunk();

	void buildModel();
	//void buildModelThreaded(std::vector<glm::vec3> *modptr, std::vector<glm::vec2> *uvptr, std::vector<int> *btiptr);
	void draw(Shader& s);

	void generateChunk(const int minHeight, const int maxHeight, const glm::vec3& poff);

protected:
	void moveChunkWorldSpace(const glm::vec2& newPos) { chunkPosxz = newPos; }

	Block& getBlockAt(unsigned int x, unsigned int y, unsigned int z) { return blocksxyz[x][y][z]; }
	Block& getBlockAt(glm::ivec3 pos) { return blocksxyz[pos.x][pos.y][pos.z]; }
	void moveBlockTo(glm::ivec3 blockPos, glm::ivec3 where);

	void sendModelDataToGL(const std::vector<glm::vec3>& model, const std::vector<glm::vec2>& uvs, const std::vector<int>& blockTypeIndices);
	
	const std::string to_string() { return "Chunk: " + std::to_string(vao) + " Coords: " + glm::to_string(chunkPosxz); }

	bool isInPlayerView = true;

private:
	glm::vec2 chunkPosxz; // all chunks are shifted by their "world" position
	std::vector<std::vector<std::vector<Block>>> blocksxyz;

	enum
	{
		UVS = 0,
		VERTS,
		BLOCK_TYPES,

		NUM_VBOS
	};

	unsigned int VBOS[NUM_VBOS];
	unsigned int vao;
	static unsigned int modelLoc;

	unsigned int numVerts;
};

#endif // CHUNK_H