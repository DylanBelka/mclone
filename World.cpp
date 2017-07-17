#include "World.h"

World::World()
{

}

Block& World::getBlockAt(const glm::vec3& pos)
{
	Chunk* thisChunk = &chunks[getChunkIndex(pos)];

	glm::ivec3 bpos((int)pos.x % chunkWidth, (int)pos.y % chunkHeight, (int)pos.z % chunkDepth);

	return thisChunk->getBlockAt(bpos);
}

int World::getChunkIndex(const glm::vec3& pos)
{
	glm::ivec2 chunkPosxz(pos.x / chunkWidth, pos.z / chunkDepth);

	std::cout << chunkPosxz.y * numRows + chunkPosxz.x << std::endl;

	return chunkPosxz.y * numRows + chunkPosxz.x;
}

void World::addNewChunk(World* w, int x, int z)
{
	Chunk c;
	c.generateChunk(10);
	c.buildModel();
	c.moveChunkWorldSpace(glm::vec2(x, z));
	static std::mutex mut;
	mut.lock();
	w->chunks.push_back(c);
	mut.unlock();
}

void World::generateWorld(int numChunks)
{
	numRows = sqrt(numChunks); // space chunks out in a grid of sqrt(numChunks) x sqrt(numChunks)
	this->numChunks = numChunks;

	using namespace std::chrono;

	const int totalSysThreads = std::thread::hardware_concurrency();
	std::cout << "System can support: " << totalSysThreads << " threads\n";

	if (totalSysThreads == 0)
	{
		std::cout << "std::thread::hardware_concurrency() poorly defined, using non threaded world generation\n";
		generateWorldNoThread(numChunks);
		return;
	}
	else if (totalSysThreads == 1)
	{
		std::cout << "system only has one thread, using non multi-threaded world generation\n";
		generateWorldNoThread(numChunks);
		return;
	}

	if (numRows * numRows != numChunks || numChunks % 2 != 0) // threaded version does not work well with odd number of chunks and non perfect squares
	{
		std::cout << "using non threaded world generation\n";
		generateWorldNoThread(numChunks);
		return;
	}

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	beginFrame = steady_clock::now();

	int x, z;
	x = z = 0;

	std::vector<std::thread> threads(totalSysThreads - 1); // one thread is the main thread

	for (int i = 0; i < numChunks; i += totalSysThreads)
	{
		for (int j = 0; j < totalSysThreads - 1; j++)
		{
			threads[j] = std::thread(addNewChunk, this, x, z);
			x += chunkWidth;
			if (x >= chunkWidth * numRows)
			{
				x = 0;
				z += 16;
			}
		}

		addNewChunk(this, x, z); // begin work on main thread

		x += chunkWidth;
		if (x >= chunkWidth * numRows)
		{
			x = 0;
			z -= 16;
		}

		for (int j = 0; j < totalSysThreads - 1; j++)
		{
			threads[j].join();
		}
	}

	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done generating world took: " << elapsed.count() << std::endl;
}

void World::generateWorldNoThread(int numChunks)
{
	numRows = sqrt(numChunks); // space chunks out in a grid of sqrt(numChunks) x sqrt(numChunks)
	this->numChunks = numChunks;

	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	beginFrame = steady_clock::now();

	int x, z;
	x = z = 0;

	for (int i = 0; i < numChunks; i++)
	{
		addNewChunk(this, x, z); // generate the chunk

		x += chunkWidth;
		if (x >= chunkWidth * numRows)
		{
			x = 0;
			z -= 16;
		}
	}

	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done generating world took: " << elapsed.count() << std::endl;
}

void World::draw(Shader& s)
{
	for (Chunk& c : chunks)
	{
		c.draw(s);
	}
}