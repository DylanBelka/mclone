#include "World.h"

World::World()
{

}

int World::getChunkIndex(const glm::vec3& pos)
{
	glm::ivec2 chunkPosxz(pos.x / chunkWidth, pos.z / chunkDepth);

	//std::cout << "pos: " << glm::to_string(pos) << std::endl;
	//std::cout << "chunkposxz: " << glm::to_string(chunkPosxz) << std::endl;
	//std::cout << "chunk coord: " << chunkPosxz.y * numRows + chunkPosxz.x << std::endl;
	//system("pause");

	int index = chunkPosxz.y * numRows + chunkPosxz.x;
	//std::cout << "unmodified cindex: " << index << "\n";
	//if (index < 0)
	//{
	//	index = -index * 2 - 1;
	//}
	//else
	//{
	//	index = index * 2;
	//}
	//std::cout << "cindex: " << index << "\n";
	return index;
}

Block& World::getBlockAt(const glm::vec3& pos)
{
	Chunk* thisChunk = &chunks[getChunkIndex(pos)];

	glm::ivec3 bpos((int)pos.x % chunkWidth, (int)pos.y % chunkHeight, (int)pos.z % chunkDepth);

	return thisChunk->getBlockAt(bpos);
}

bool World::destroyBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const float maxReach)
{
	glm::vec3 searchPos = startingPos;

	while (glm::distance(searchPos, startingPos) < maxReach)
	{
		searchPos += front;

		if (getBlockAt(searchPos).type != Block::AIR)
		{
			getBlockAt(searchPos).type = Block::AIR;
			return true;
		}
	}

	return false; // could not find a destroyable block
}

bool World::placeBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const Block::BlockType type, const float maxReach)
{
	glm::vec3 searchPos = startingPos;

	while (glm::distance(searchPos, startingPos) < maxReach)
	{
		searchPos += front;

		if (getBlockAt(searchPos).type != Block::AIR)
		{
			while (getBlockAt(searchPos).type != Block::AIR) // traverse back until there is an air block to place on
			{
				searchPos -= front;
			}
			getBlockAt(searchPos).type = type;
			return true;
		}
	}

	return false; // could not place block
}

void World::generateWorld(int numChunks, int seed)
{
	numRows = sqrt(numChunks); // space chunks out in a grid of sqrt(numChunks) x sqrt(numChunks)
	this->numChunks = numChunks;

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

#ifdef DEBUG
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	beginFrame = steady_clock::now();
#endif // DEBUG

	int x, z;
	x = z = 0;
	float px, pz; // perlin offset coords, really just chunk coords
	float py;
 	int pxstart;

	//std::random_device r; // random_device for seeding the Mersenne Twister 19937 generator, slower than mt19937 but "more random"
	std::mt19937 eng(seed);
	std::uniform_int_distribution<int> distrInt(1, 10); // for seeding perlin noise
	std::uniform_real_distribution<float> distrFloat(.01, .03); // for varying increments of perlin noise crawling

	// randomize the starting position of the perlin noise
	// essentially acting as a "seed" for the perlin noise generator
	px = distrInt(eng);
	pz = distrInt(eng);
	py = distrFloat(eng);
	pxstart = px;
	std::cout << "px: " << px << " pz: " << pz << std::endl;

	std::vector<std::thread> threads(totalSysThreads); // one thread is the main thread

	//const float incMaxHeight = .003;

	for (int i = 0; i < numChunks; i += totalSysThreads)
	{
		std::vector<Chunk> tempChunks; // chunks that will be made during this loop of totalSysThread
		std::vector<std::vector<glm::vec3>> allFinalModels(totalSysThreads);
		std::vector<std::vector<glm::vec2>> alluvs(totalSysThreads);
		std::vector<std::vector<int>> allBlockTypeIndices(totalSysThreads);
		
		for (int j = 0; j < totalSysThreads; j++)
		{
			Chunk tmp;
			tempChunks.push_back(tmp);
		}

		for (int j = 0; j < totalSysThreads; j++)
		{
			//float noise = glm::perlin(glm::vec2(px * incMaxHeight, pz * incMaxHeight));
			//int maxHeight = map(noise, -1.0, 1.0, 10.0, 250.0);

			tempChunks[j].generateChunk(1, 15, glm::vec3(px, py, pz)); 
			threads[j] = std::thread(&Chunk::buildModelThreaded, &tempChunks[j], &allFinalModels[j], &alluvs[j], &allBlockTypeIndices[j]);

			px += chunkWidth;
			if (px >= chunkWidth * numRows)
			{
				//px = pxstart;
				pz -= 16;
			}
		}

		for (std::thread& t : threads)
		{
			t.join();
		}

		for (int j = 0; j < totalSysThreads; j++)
		{
			tempChunks[j].sendModelDataToGL(allFinalModels[j], alluvs[j], allBlockTypeIndices[j]);
			tempChunks[j].moveChunkWorldSpace(glm::vec2(x, z));
			x += chunkWidth;
			if (x >= chunkWidth * numRows)
			{
				x = 0;
				z += 16;
			}

			chunks.push_back(tempChunks[j]);
		}
	}
#ifdef DEBUG
	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done generating world took: " << elapsed.count() << std::endl;
#endif // DEBUG
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
	int px, pz;
	float py;
	x = z = 0;
	px = pz = 0;

	std::random_device r; // random_device for seeding the Mersenne Twister 19937 generator, slower than mt19937 but "more random"

	for (int i = 0; i < numChunks; i++)
	{
		Chunk c;

		c.generateChunk(1, 15, glm::vec3(px, pz, py));
		px += chunkWidth;
		if (px >= chunkWidth * numRows)
		{
			px = 0;
			pz += 16;
		}

		c.buildModel();
		c.moveChunkWorldSpace(glm::vec2(x, z));

		//std::cout << "pushing chunk: " << chunks.size() << std::endl;
		chunks.push_back(c);

		x += chunkWidth;
		if (x >= chunkWidth * numRows)
		{
			x = 0;
			z += 16;
		}
	}

	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done generating world, nonthreaded, took: " << elapsed.count() << std::endl;
}

void World::update(const glm::vec3& playerPos, const unsigned int chunkRenderDistance)
{
	for (int i = 0; i < numChunks; i++)
	{
		Chunk* chk = &chunks[i];
		glm::vec2 chunkPos(chk->chunkPosxz);
		if (glm::distance(chunkPos.x, playerPos.x) > sqrt(chunkRenderDistance) * chunkWidth) // check x
		{
			chk->generateChunk(1, 15, glm::vec3(0.0, 0.0, 0.0));
			chk->buildModel();
			float newX = chunkPos.x;
			for (int j = 0; j < numRows; j++)
			{
				newX += chunkWidth;
			}
			chk->moveChunkWorldSpace(glm::vec2(newX, chunkPos.y));
			std::cout << "x" << std::endl;
		}
		if (glm::distance(chunkPos.y, playerPos.z) > sqrt(chunkRenderDistance) * chunkDepth) // check z
		{
			chk->generateChunk(1, 15, glm::vec3(0.0, 0.0, 0.0));
			chk->buildModel();
			float newZ = chunkPos.y;
			for (int j = 0; j < numRows; j++)
			{
				newZ += chunkDepth;
			}
			chk->moveChunkWorldSpace(glm::vec2(chunkPos.x, newZ));
			std::cout << "z" << std::endl;

		}
	}
}

void World::draw(Shader& s)
{
	for (Chunk& c : chunks)
	{
		c.draw(s);
	}
}