#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <cmath>
#include <chrono>

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

	void generateWorld(int numChunks); // numChunks should always be a perfect square
	void generateWorldNoThread(int numChunks); // if the system only has one thread for some reason or std::thread::hardware_concurrency() is poorly defined

	Block& getBlockAt(const glm::vec3& pos);
	void draw(Shader& s);

	int getChunkIndex(const glm::vec3& pos);
	void updateChunk(int cid) { chunks[cid].buildModel(); }

	unsigned int getNumChunks() { return numChunks; }

private:
	static void addNewChunk(World* w, int x, int z);

	std::vector<Chunk> chunks;

	unsigned int numRows;
	unsigned int numChunks;
};

#endif // WORLD_H