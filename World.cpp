#include "World.h"

World::World()
{

}

const Block& World::getBlockAt(const glm::vec3& pos)
{
	
}

void World::addNewChunk(World* w, int x, int z)
{
	Chunk c;
	c.generateChunk(time(NULL));
	c.buildModel();
	c.moveChunkWorldSpace(glm::vec2(x, z));
	static std::mutex mut;
	mut.lock();
	w->chunks.push_back(c);
	mut.unlock();
}

void World::generateWorld(int numChunks)
{
	int w = sqrt(numChunks); // space chunks out in a grid of sqrt(numChunks) x sqrt(numChunks)

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
			if (x >= chunkWidth * w)
			{
				x = 0;
				z -= 16;
			}
		}

		addNewChunk(this, x, z); // begin work on main thread

		x += chunkWidth;
		if (x >= chunkWidth * w)
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
	int w = sqrt(numChunks); // space chunks out in a grid of sqrt(numChunks) x sqrt(numChunks)

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
		if (x >= chunkWidth * w)
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