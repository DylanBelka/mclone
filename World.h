#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <cmath>
#include <chrono>
#include <random>

#include <thread>
#include <mutex>

#include <glm.hpp>

#include "Shader.h"
#include "Chunk.h"
#include "Block.h"

class World
{
public:
	World();

	void draw(Shader& s);

	void generateWorld(int numChunks, int seed = 0); // numChunks should always be a perfect square
	void generateWorldNoThread(int numChunks); // if the system only has one thread for some reason or std::thread::hardware_concurrency() is poorly defined

	Block& getBlockAt(const glm::vec3& pos);
	bool destroyBlockAt(const glm::vec3& startingPos, const glm::vec3& front); // returns true if the block could be destroyed
	

	int getChunkIndex(const glm::vec3& pos);
	void updateChunk(int cid) { chunks[cid].buildModel(); }

	unsigned int getNumChunks() { return numChunks; }

private:
	Block& getBlockReach(const glm::vec3& startingPos, const glm::vec3& front);

	std::vector<Chunk> chunks;

	unsigned int numRows;
	unsigned int numChunks;
};

#endif // WORLD_H