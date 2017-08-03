#ifndef WORLD_H
#define WORLD_H

#include <cmath>
#include <chrono>
#include <random>

#include <unordered_map>

#include <thread>
#include <mutex>

#include <glm.hpp>
#include <gtx/hash.hpp>

#include "Shader.h"
#include "Chunk.h"
#include "Block.h"
#include "math.h"

class World
{
public:
	World();

	void draw(Shader& s);

	//void generateWorld(int numChunks, int seed = 0); // numChunks should always be a perfect square
	//void generateWorldNoThread(int numChunks); // if the system only has one thread for some reason or std::thread::hardware_concurrency() is poorly defined

	Block& getBlockAt(const glm::vec3& pos);
	bool destroyBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const float maxReach = 4.0); // returns true if the block could be destroyed
	bool placeBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const Block::BlockType type, const float maxReach = 4.0); // returns true if the block could be placed

	void updateChunk(glm::vec2 chunkCoord) { hchunks[chunkCoord].buildModel(); }
	void deleteChunk(glm::vec2 chunkCoord) { hchunks[chunkCoord].deleteChunk(); }

	void updateChunk(glm::vec3 worldPos) { hchunks[worldCoordToChunkCoord(worldPos)].buildModel(); }
	void deleteChunk(glm::vec3 worldPos) { hchunks[worldCoordToChunkCoord(worldPos)].deleteChunk(); }

	unsigned int getNumChunks() { return numChunks; }

	void update(const glm::vec3& playerPos, const unsigned int renderDistance);

private:
	glm::ivec2 worldCoordToChunkCoord(const glm::vec3& pos) { return glm::ivec2(pos.x / chunkWidth, pos.z / chunkDepth) * 16; }
	void createChunk(glm::vec2 chunkPos, glm::vec3 perlinCoords);

private:
	std::unordered_map<glm::vec2, Chunk> hchunks;

	unsigned int numRows;
	unsigned int numChunks;
};

#endif // WORLD_H