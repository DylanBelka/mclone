#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <memory>
#include <iostream>

#include <chrono>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>

#include "Block.h"
#include "Shader.h"

const unsigned int chunkWidth = 16;
const unsigned int chunkHeight = 256;
const unsigned int chunkDepth = 16;

const unsigned int chunkSize = chunkWidth * chunkDepth * chunkHeight;

namespace Face {
	enum FaceIndex
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,

		NUM_FACES
	};
}

/****

	A chunk holds 16 * 16 * 256 blocks 
	Handles converting std::vector<Block> into a single chunk mesh
	For faster drawing + fewer draw calls
	
****/

class Chunk
{
public:
	Chunk();
	~Chunk();

	void buildModel();

	void moveBlockTo(glm::ivec3 blockPos, glm::ivec3 where);
	Block& getBlock(unsigned int x, unsigned int y, unsigned int z) { return blocksxyz[x][y][z]; }

	void draw(Shader& s);

	const glm::vec3 getBlockWorldPosition(glm::ivec3 blockPos) { return glm::vec3(chunkPosxz.x + blockPos.x, blockPos.y, chunkPosxz.y + blockPos.z); }

	static Chunk createChunk();

private:
	void sendModelDataToGL(const std::vector<glm::vec3>& model, const std::vector<glm::vec2>& uvs);
	
	std::vector<std::vector<std::vector<Block>>> blocksxyz;
	glm::vec2 chunkPosxz; // all chunks are shifted by their "world" position

	enum
	{
		UVS = 0,
		VERTS,

		NUM_VBOS
	};

	unsigned int VBOS[NUM_VBOS];
	unsigned int vao;
	unsigned int modelLoc = -1;

	unsigned int numVerts;
};

#endif // CHUNK_H